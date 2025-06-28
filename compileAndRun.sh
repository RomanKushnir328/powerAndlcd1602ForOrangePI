#!/bin/bash

SCRIPT_PATH=$(realpath "$0")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")
cd $SCRIPT_DIR

rm -rf build
mkdir build
cd build
cmake ..
make
make install
cd ..

if [ ! -f "displayManagement/autostart.service" ]; then
	sudo tee "displayManagement/autostart.service" > /dev/null <<EOF
[Unit]
Description=Display management
After=local-fs.target

[Service]
ExecStart=/usr/local/bin/displayManagement
Restart=always

[Install]
WantedBy=multi-user.target
EOF
fi

if [ ! -f "powerManagement/autostart.service" ]; then
	sudo tee "powerManagement/autostart.service" > /dev/null <<EOF
[Unit]
Description=Power management
After=local-fs.target

[Service]
ExecStart=/usr/local/bin/displayManagement
Restart=always

[Install]
WantedBy=multi-user.target
EOF
fi

cp displayManagement/autostart.service /etc/systemd/system/displayManagement.service
cp powerManagement/autostart.service /etc/systemd/system/powerManagement.service

sudo systemctl daemon-reexec
sudo systemctl daemon-reload
sudo systemctl enable "displayManagement.service"
sudo systemctl start "displayManagement.service"
sudo systemctl enable "powerManagement.service"
sudo systemctl start "powerManagement.service"

echo "Finish"
