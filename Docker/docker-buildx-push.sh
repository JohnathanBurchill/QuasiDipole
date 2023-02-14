docker buildx build --platform linux/amd64,linux/arm64,linux/arm/v7 --no-cache --target deploy -t johnathanburchill/quasidipole:latest --push .
