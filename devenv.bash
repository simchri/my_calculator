#!/usr/bin/env bash

function script_body() {
  # strict bash:
  set -o errexit
  set -o nounset
  set -o pipefail

  parse_args "$@"

  if [ "$HELP" == 1 ]; then
    show_help
    return 0
  fi

  echo "Ensure any user can access docker daemon"
  files="
  /var/run/docker.sock
  /var/run/docker.pid"
  any_user_can_rwx "$files"

  if [ -z "$UID" ]; then
    UID=$(id -u)
  fi
  export UID

  if [ "$BUILD" == 1 ]; then
    # prompt user if they really want to rebuild:
    prompt_text="\n!!!\nYou are about to REBUILD the docker image.\nCHANGES TO THE CONTAINER WILL BE LOST. \
\n!!!\n"
    echo -e "$prompt_text"
    prompt_text="ANY KEY to continue, Ctrl-C to exit."
    read -p "$prompt_text" -n 1 -r
    docker compose down || {
      echo "failed to clean up old container(s). exiting."
      return 1
    }
    docker compose build || {
      echo "failed to build new image. exiting."
      return 1
    }
  fi

  docker compose up -d
  echo "open container shell ..."

  # 'dev-container' = service name as defined in compose file.
  # 'my_calc' = name of directory containing compose file
  docker exec -it my_calc-dev-container-1 bash || true # evade set -e, to always print goodbye message
  printf "\nclosing dev-env shell. You may want to stop the docker container process with\n\
docker compose stop \n"
}

function parse_args() {

  POSITIONAL_ARGS=()
  HELP=0
  BUILD=0

  while [[ $# -gt 0 ]]; do
    case $1 in
      -h | --help)
        HELP=1
        shift
        ;;
      -b | --build)
        BUILD=1
        shift
        ;;
      -*)
        echo "Unknown option $1"
        return 1
        ;;
      *)
        POSITIONAL_ARGS+=("$1") # save positional arg
        shift                   # past argument
        ;;
    esac
  done

  if [ ${#POSITIONAL_ARGS[@]} -gt 0 ]; then
    echo "Too many positional arguments!"
    return 1
  fi
}

function any_user_can_rwx() {
  files="$1"
  first=1
  for file in $files; do
    permissions=$(stat -c "%a" "$file")
    if [ "${permissions}" -ne 777 ]; then
      if [ "$first" == 1 ]; then
        echo "sudo 'chmod a+rwx' for files: $1"
        first=0
      fi
      sudo chmod a+rwx "$file"
    fi
  done
}

function show_help() {
  echo ""
  echo "./devenv.bash [--build]"
  echo ""
  echo "Open the docker compose development environment. Optionally rebuild image."
  echo ""
  echo "This is a helper script over 'docker compose'. You can also just use plain docker (compose) commands!"
  echo ""
  echo "Options:"
  echo "  -h, --help: show this help"
  echo "  -b, --build: (Re-) build docker image, re-create container (runs 'docker compose build')"
}

if [ "${BASH_SOURCE[0]}" != "$0" ]; then
  echo "You should execute this script as a child bash process, not source it!"
  return 1
else
  script_body "$@"
fi
