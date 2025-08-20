#!/bin/sh
#
# modules 변수에 "모듈명|인자|폴더종류" 형식으로 입력
# 빌드 제외하려면 # 주석 처리
#

modules="
ap_sm|-c -p|ap_app
ap_ipchandler||ap_app
#ap_soa_wiper||ap_app
#ap_intelligent_wiper||subscription_app
#ap_katech||subscription_app
#ap_lotte_service||subscription_app
#ap_service_creator||subscription_app
#ap_sesl_service||subscription_app
#ap_tsmart||subscription_app
"

echo "$modules" | while IFS="|" read -r dir args folder; do
    # 빈 줄 또는 주석(#)은 건너뛰기
    [ -z "$dir" ] && continue
    case "$dir" in
        \#*) continue ;;
    esac

    BASE_DIR="$(pwd)/$folder"
    MODULE_DIR="$BASE_DIR/$dir"
    BUILD_SCRIPT="$MODULE_DIR/docker_build.sh"

    if [ ! -d "$MODULE_DIR" ]; then
        echo "⚠️  Directory $MODULE_DIR does not exist. Skipping build for $dir."
        continue
    fi

    if [ ! -x "$BUILD_SCRIPT" ]; then
        echo "⚠️  Build script $BUILD_SCRIPT not found or not executable. Skipping build for $dir."
        continue
    fi

    echo ">> Building $dir (folder: $folder)"
    bash "$BUILD_SCRIPT" $args
done

