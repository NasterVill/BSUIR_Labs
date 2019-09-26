import math
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image, ImageFilter
from skimage import io, measure
from scipy.cluster.vq import kmeans


# can identify up to 255 objects
def mark_objects(image: Image.Image):
    new_image = image.copy()

    current = 0
    free_labels = []

    pixels = new_image.load()
    for x in range(0, new_image.size[0]):
        for y in range(0, new_image.size[1]):
            kn = y - 1
            if kn <= 0:
                B = 0
            else:
                B = pixels[x, kn]

            km = x - 1
            if km <= 0:
                C = 0
            else:
                C = pixels[km, y]

            A = pixels[x, y]

            if A != 0:
                if B == 0 and C == 0:
                    if len(free_labels) > 0:
                        label = free_labels.pop()
                    else:
                        current = current + 1
                        label = current

                    pixels[x, y] = label
                elif B != 0 and C == 0:
                    pixels[x, y] = B
                elif B == 0 and C != 0:
                    pixels[x, y] = C
                elif B != 0 and C != 0:
                    pixels[x, y] = B
                    if B != C:
                        change_label(new_image, C, B)
                        free_labels.append(C)

    return new_image


def change_label(image, old_label: int, new_label: int):
    pixels = image.load()

    for x in range(image.size[0]):
        for y in range(image.size[1]):
            if pixels[x, y] == old_label:
                pixels[x, y] = new_label

    return


def binarization(image, threshold):
    pixels = image.load()

    for i in range(image.size[0]):
        for j in range(image.size[1]):
            pixels[i, j] = 0 if pixels[i, j] <= threshold else 255

    return


def generate_unique_values(image, transform_colors=False):
    matrix = np.array(image.getdata()).reshape(image.size)

    # list of all the unique labels, associated with different objects
    # i.e. their colors in grayscale image
    unique_labels = np.unique(matrix)

    # remove zero, because it is a background
    unique_labels = np.delete(unique_labels, 0)

    if len(unique_labels) < 25 and transform_colors:
        for i in range(0, len(unique_labels)):
            change_label(image, int(unique_labels[i]), int(unique_labels[i] * 10))
            unique_labels[i] *= 10

    return unique_labels


def plot_results(vectors, centroids, amount_of_clusters):
    ox = [vector[0] for vector in vectors]
    oy = [vector[1] for vector in vectors]

    fig = plt.figure()
    ax = fig.subplots()

    ax.scatter(ox, oy, c='r', marker='x')

    for i in range(0, len(ox)):
        ax.annotate(
            'Object {}'.format(i + 1),
            xy=(ox[i], oy[i]),
            xytext=(ox[i] + 0.05 * i, oy[i] + 0.05 * i),
            arrowprops=dict(
                arrowstyle="->",
                connectionstyle='angle3',
            ),
        )

    for i in range(amount_of_clusters):
        ax.scatter(
            centroids[i][0],
            centroids[i][1],
            c='g', marker='o'
        )

    ax.set_xlabel('Area')
    ax.set_ylabel('Eccentricity')
    plt.show()


def get_region_vectors_skimage(image_name):
    sk_image = io.imread(image_name)

    label_image = measure.label(sk_image)

    regions = measure.regionprops(
        label_image
    )

    fig, ax = plt.subplots()
    ax.imshow(sk_image)

    j = 0
    for props in regions:
        j += 1
        y0, x0 = props.centroid
        orientation = props.orientation
        x1 = x0 + math.cos(orientation) * 0.5 * props.major_axis_length
        y1 = y0 - math.sin(orientation) * 0.5 * props.major_axis_length
        x2 = x0 - math.sin(orientation) * 0.5 * props.minor_axis_length
        y2 = y0 - math.cos(orientation) * 0.5 * props.minor_axis_length

        ax.plot((x0, x1), (y0, y1), '-r', linewidth=2.5)
        ax.plot((x0, x2), (y0, y2), '-r', linewidth=2.5)
        ax.plot(x0, y0, '.g', markersize=15)

        ax.annotate(
            'Object {}'.format(j), xy=(x0, y0), arrowprops=dict(facecolor='black', shrink=0.05),
        )

        minr, minc, maxr, maxc = props.bbox
        bx = (minc, maxc, maxc, minc, minc)
        by = (minr, minr, maxr, maxr, minr)
        ax.plot(bx, by, '-b', linewidth=2.5)

    regions_vectors = []
    for i in range(0, len(regions)):
        regions_vectors.append([regions[i].area, regions[i].eccentricity])
        print(regions[i].label)

    return regions_vectors


def main():
    initial_image_name = 'images/P0001465.jpg'
    final_image_name = 'images/final.pgm'

    # open image
    initial_image = Image.open(initial_image_name)
    image = initial_image.copy().convert("L")

    plt.imshow(image)
    plt.show()

    binarization_threshold = 200

    # initial binarization
    binarization(image, binarization_threshold)

    plt.imshow(image)
    plt.show()

    # filtering for noise reduction
    new_image = image.filter(ImageFilter.MedianFilter(size=5))

    # one more binarization to nullify median filtering "averaging" effect on borders
    binarization(new_image, binarization_threshold)

    plt.imshow(new_image)
    plt.show()

    # reset 255 to 1 in order to make unique labels sequence consistent and
    # all its members have minimal values
    change_label(new_image, 255, 1)

    # mark objects
    marked_image = mark_objects(new_image)

    unique_labels = generate_unique_values(marked_image, transform_colors=True)

    print('Unique labels:', unique_labels)

    # save marked result
    marked_image.save(final_image_name)

    plt.imshow(marked_image)
    plt.show()

    # solution with skimage instead of manual computing
    regions_vectors = get_region_vectors_skimage(final_image_name)

    centroids, _ = kmeans(np.array(regions_vectors), 2)
    plot_results(regions_vectors, centroids, 2)

    plt.show()


if __name__ == '__main__':
    main()
