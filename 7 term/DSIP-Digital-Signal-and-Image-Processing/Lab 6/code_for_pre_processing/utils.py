import pathlib
import pandas as pd
import random
from PIL import Image
from resizeimage import resizeimage


def generate_dataset_metadata(path_to_dataset: str = '.', extension: str = 'png', table_file_name: str = 'exodus_table.txt'):
    bad_class = '14'

    dataset = []

    with open(table_file_name, 'r') as file:
        line = file.readline()
        while line:
            data = line.split('\t')

            image_name = data[0]

            try:
                path = pathlib.Path(f'{path_to_dataset}/{image_name}.{extension}')
                test_file = open(path)
                test_file.close()
            except:
                print(f'No such file: {image_name}! Skipping...')

                line = file.readline()

                continue

            labels = data[1].split(' ')

            # filter out deseases' names
            acc = []
            for element in labels:
                if element != '':
                    acc.append(element)
                else:
                    break
            labels = acc

            # filter out bad class
            if bad_class in labels:
                if len(labels) > 1:
                    labels.remove(bad_class)
                else:
                    line = file.readline()

                    continue

            data = [image_name, labels]

            dataset.append(data)

            line = file.readline()

    return dataset


def create_data_frame(dataset: list, name):
    df = pd.DataFrame(
        data={
            'image_name': map(lambda x: x[0], dataset),
            'labels': map(lambda x: ' '.join(label for label in x[1]), dataset)
        }
    )
    df.to_csv(name, sep=',', index=False)


def transform_images(image_names, path_to_dataset: str = '.'):
    for image_name in image_names:
        path = pathlib.Path(f'{path_to_dataset}/{image_name}.ppm')

        with open(path, 'r+b') as file:
            with Image.open(file) as image:
                cover = resizeimage.resize_contain(image, [700, 700])

                cover = resizeimage.resize_contain(cover, [256, 256])

                cover.save(f'./transformed/{image_name}.png')


def generate_random_sets(max: int, set_size: int):
    numbers = list(range(max))

    n_sets = int(max / set_size)

    sets = []

    for i in range(n_sets):
        tmp_set = random.sample(numbers, k=set_size)

        for j in tmp_set:
            numbers.remove(j)

        sets.append(tmp_set)

        print(i, tmp_set)

    return sets