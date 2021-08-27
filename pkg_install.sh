#!/bin/bash

echo "Install needed packages..."
sudo apt install -y build-essential python3 python3-pip python3-opencv wget libjpeg-dev zlib1g-dev libpython3-dev libavcodec-dev libavformat-dev libswscale-dev libopenblas-dev
echo "Download pytorch wheel"
wget https://nvidia.box.com/shared/static/p57jwntv436lfrd78inwl7iml6p13fzh.whl -O torch-1.8.0-cp36-cp36m-linux_aarch64.whl
echo "Install pytorch wheel"
pip3 install Cython numpy torch-1.8.0-cp36-cp36m-linux_aarch64.whl
git clone --branch "v0.9.0" --depth 1 https://github.com/pytorch/vision torchvision
cd torchvision
export BUILD_VERSION=0.9.0
python3 setup.py install --user
cd ../

pip3 install pyserial
