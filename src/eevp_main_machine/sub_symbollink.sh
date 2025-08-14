#!/bin/sh

TARGET="adaptive_autosar"

apps="ap_katech ap_intelligent_wiper ap_lotte_service ap_service_creator ap_sesl_service ap_tsmart"

BASE_DIR="$(pwd)/subscription_app"

for app in $apps; do
  LINK_DIR="$BASE_DIR/$app"

  if [ -d "$LINK_DIR" ]; then
    echo "Updating symlink in $LINK_DIR"
    rm -rf "$LINK_DIR/$TARGET"
    ln -s ../../$TARGET "$LINK_DIR/$TARGET"
  else
    echo "Warning: Directory $LINK_DIR not found, skipping."
  fi
done
