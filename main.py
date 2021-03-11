import numpy as np
from PIL import Image
from perlin_noise import PerlinNoise
import matplotlib.pyplot as plt


def generate_noise_map(width, height):
    noise_l0 = PerlinNoise(octaves=3)
    noise_l1 = PerlinNoise(octaves=6)
    noise_l2 = PerlinNoise(octaves=12)

    warp_l0 = PerlinNoise(octaves=6)
    warp_l1 = PerlinNoise(octaves=12)
    warp_l2 = PerlinNoise(octaves=24)

    noise_map = np.zeros((height, width), dtype=np.float64)

    warp_const = 3
    for y in range(height):
        for x in range(width):
            warp_x = warp_l0(x / width)
            warp_y = warp_l0(y / height)
            point = [(x + warp_x * warp_const) / width,
                     (y + warp_y * warp_const)/ height]
            val = noise_l0(point)

            warp_x = warp_l1(x / width)
            warp_y = warp_l1(y / height)
            point = [(x + warp_x * warp_const) / width,
                     (y + warp_y * warp_const) / height]
            val += 0.5 * noise_l1(point)

            warp_x = warp_l2(x / width)
            warp_y = warp_l2(y / height)
            point = [(x + warp_x * warp_const) / width,
                     (y + warp_y * warp_const) / height]
            val += 0.25 * noise_l2(point)

            noise_map[y][x] = val

    return noise_map


def main():
    n = generate_noise_map(100, 100)

    plt.imshow(n, cmap='gray')
    plt.show()


if __name__ == '__main__':
    main()
