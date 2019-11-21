import os
import random
from scipy import ndarray

# image processing library
import skimage as sk
from skimage import transform
from skimage import util
from skimage import io
import pathlib


def random_rotation(image_array: ndarray):
    # pick a random degree of rotation between 30% on the left and 30% on the right
    random_degree = random.uniform(-30, 30)
    return sk.transform.rotate(image_array, random_degree)


def random_noise(image_array: ndarray):
    # add random noise to the image
    return sk.util.random_noise(image_array)


def horizontal_flip(image_array: ndarray):
    # horizontal flip doesn't need skimage, it's easy as flipping the image array of pixels !
    return image_array[:, ::-1]


def horizontal_flip(image_array: ndarray):
    # horizontal flip doesn't need skimage, it's easy as flipping the image array of pixels !
    return image_array[:, ::-1]


# dictionary of the transformations we defined earlier
available_transformations = {
    'rotate': random_rotation,
    'horizontal_flip': horizontal_flip
}

folder_path = 'transformed'
num_files_desired = 140

# find all files paths from the folder
# list(map(lambda image_name: pathlib.Path(f'{folder_path}/{image_name}'), os.listdir(folder_path)))
images = os.listdir(folder_path)

print(images)

num_generated_files = 0
while num_generated_files <= num_files_desired:
    # random image from the folder
    image_name = random.choice(images)
    images.remove(image_name)

    # read image as an two dimensional array of pixels
    image_to_transform = sk.io.imread(pathlib.Path(f'{folder_path}/{image_name}'))
    # random num of transformation to apply
    num_transformations_to_apply = random.randint(1, len(available_transformations))

    num_transformations = 0
    transformed_image = None
    while num_transformations <= num_transformations_to_apply:
        # random transformation to apply for a single image
        key = random.choice(list(available_transformations))
        transformed_image = available_transformations[key](image_to_transform)
        num_transformations += 1

    new_file_path = f'{folder_path}/augmented_{image_name}'

    # write image to the disk
    io.imsave(new_file_path, transformed_image)
    num_generated_files += 1