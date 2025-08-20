#!/bin/sh

TARGET="adaptive_autosar"

# subscription_app 안에 있는 앱 목록
apps_sub="ap_katech ap_intelligent_wiper ap_lotte_service ap_service_creator ap_sesl_service ap_tsmart"

# ap_app 안에 있는 앱 목록
apps_ap="ap_ipchandler ap_sm ap_soa ap_soa_debugger ap_soa_dms ap_soa_driverseat ap_soa_hvac ap_soa_moodlamp ap_soa_power ap_soa_rearcurtain ap_soa_roa ap_soa_smartfilm ap_soa_wiper ap_subscriptionmanager"

BASE_DIR_SUB="$(pwd)/subscription_app"
BASE_DIR_AP="$(pwd)/ap_app"

# subscription_app 처리
for app in $apps_sub; do
  LINK_DIR="$BASE_DIR_SUB/$app"
  if [ -d "$LINK_DIR" ]; then
    echo "Updating symlink in $LINK_DIR"
    rm -rf "$LINK_DIR/$TARGET"
    ln -s ../../$TARGET "$LINK_DIR/$TARGET"
  else
    echo "Warning: Directory $LINK_DIR not found, skipping."
  fi
done

# ap_app 처리
for app in $apps_ap; do
  LINK_DIR="$BASE_DIR_AP/$app"
  if [ -d "$LINK_DIR" ]; then
    echo "Updating symlink in $LINK_DIR"
    rm -rf "$LINK_DIR/$TARGET"
    ln -s ../../$TARGET "$LINK_DIR/$TARGET"
  else
    echo "Warning: Directory $LINK_DIR not found, skipping."
  fi
done

