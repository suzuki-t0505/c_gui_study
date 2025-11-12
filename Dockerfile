FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive
# 基本ツール + GTK3 開発ヘッダ
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential gcc pkg-config \
    libgtk-3-dev libglib2.0-dev libpango1.0-dev libatk1.0-dev libcairo2-dev \
    ca-certificates locales \
  && sed -i 's/# \(ja_JP.UTF-8\)/\1/' /etc/locale.gen \
  && locale-gen \
  && rm -rf /var/lib/apt/lists/*

ENV LANG=ja_JP.UTF-8 LC_ALL=ja_JP.UTF-8
WORKDIR /work