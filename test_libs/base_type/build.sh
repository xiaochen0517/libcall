#!/bin/sh

set -e

(
  cd "$(dirname "$0")"

  # 设置变量
  OUTPUT_LIB="test.so"
  SRC_DIR="."
  CXX_FLAGS="-fPIC -Wall -O2"
  LD_FLAGS="-shared"

  # 获取所有 cpp 文件
  CPP_FILES=$(find "$SRC_DIR" -maxdepth 1 -name "*.cpp" -type f)

  # 检查是否有 cpp 文件
  if [ -z "$CPP_FILES" ]; then
    echo "[-] 错误：未找到任何 .cpp 文件"
    exit 1
  fi

  echo "[+] 找到的源文件："
  echo "$CPP_FILES"

  # 编译并链接生成动态库
  echo "[+] 开始编译 $OUTPUT_LIB ..."
  if g++ $CXX_FLAGS $LD_FLAGS -o "$OUTPUT_LIB" "$CPP_FILES"; then
    echo "[+] 编译成功：$OUTPUT_LIB"
    ls -lh "$OUTPUT_LIB"
  else
    echo "[-] 编译失败"
    exit 1
  fi
)
