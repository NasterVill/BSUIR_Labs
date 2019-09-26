from pathlib import Path
import matplotlib.pyplot as plt
from PIL import Image


def get_project_root() -> Path:
    """Returns project root folder."""
    return Path(__file__).parent


def show_hist(grayscale, num_of_bins, bins_range):
    first_bins_values, _, _ = \
        plt.hist(grayscale, num_of_bins, bins_range, facecolor='blue', edgecolor='black', linewidth=1)
    plt.show()


def show_preconfigured_hist(grayscale):
    unit = 1
    num_of_bins = int(255 / unit)
    bins_range = 0, 255

    show_hist(grayscale, num_of_bins, bins_range)


def resize_image(image, size):
    image_copy = image.copy()
    image_copy.thumbnail(size, Image.ANTIALIAS)

    return image_copy


def represents_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False
