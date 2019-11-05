import numpy as np
from sklearn.neural_network import MLPClassifier

from utils import generate_noise


def print_shape(shape: np.array):
    print('\n')
    for row in shape:
        print(' '.join(' x'[val] for val in row))
    print('\n')


def get_shapes_with_noise(initial_shape: np.array):
    noised_shapes = []

    for i in range(5, 55, 5):
        noise_level = i / 100

        noised_shapes.append((generate_noise(initial_shape, noise_level), noise_level))

    return noised_shapes


def output_prediction_result(predictions):
    class_number = 1

    print(f'Probability of membership:')

    for prediction in predictions[0]:
        print(f'Class {class_number}: {prediction}')
        class_number += 1

    print('\n\n')


def make_prediction(images_with_noise, mlp, label):
    print(f'*** Prediction for : {label} ***')

    for image_noise in images_with_noise:
        print(f'Noise percent: {image_noise[1]}')
        print_shape(image_noise[0])

        predictions = mlp.predict_proba([image_noise[0].flatten()])

        output_prediction_result(predictions)


def main():
    first_shape = np.array([
        [0, 1, 1, 1, 1, 0],
        [1, 1, 0, 0, 1, 1],
        [1, 0, 1, 1, 0, 1],
        [1, 0, 1, 1, 0, 1],
        [1, 1, 0, 0, 1, 1],
        [0, 1, 1, 1, 1, 0],
    ])
    second_shape = np.array([
        [0, 1, 1, 1, 1, 0],
        [1, 0, 1, 1, 0, 1],
        [1, 1, 1, 1, 1, 1],
        [1, 1, 1, 1, 1, 1],
        [1, 0, 1, 1, 0, 1],
        [0, 1, 1, 1, 1, 0],
    ])
    third_shape = np.array([
        [1, 0, 0, 0, 0, 1],
        [0, 1, 0, 0, 1, 0],
        [0, 0, 1, 1, 0, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 1, 0, 0, 1, 0],
        [1, 0, 0, 0, 0, 1],
    ])
    fourth_shape = np.array([
        [0, 0, 1, 1, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 1, 1, 0, 0],
    ])
    fifth_shape = np.array([
        [0, 0, 1, 1, 0, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 1, 1, 1, 1, 0],
        [0, 0, 1, 1, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 1, 1, 1, 1, 0],
    ])

    first_shape_noised = get_shapes_with_noise(first_shape)
    second_shape_noised = get_shapes_with_noise(second_shape)
    third_shape_noised = get_shapes_with_noise(third_shape)
    fourth_shape_noised = get_shapes_with_noise(fourth_shape)
    fifth_shape_noised = get_shapes_with_noise(fifth_shape)

    shapes = [
        first_shape.flatten(),
        second_shape.flatten(),
        third_shape.flatten(),
        fourth_shape.flatten(),
        fifth_shape.flatten()
    ]

    classes = [0, 1, 2, 3, 4]

    hidden_neurons = 40
    mlp = MLPClassifier(
        hidden_layer_sizes=hidden_neurons,
        learning_rate_init=0.5,
        learning_rate='adaptive',
        activation='logistic',
        max_iter=20,
        solver='sgd',
        #verbose=True
    )
    mlp.fit(shapes, classes)

    make_prediction(first_shape_noised, mlp, 'first shape')
    make_prediction(second_shape_noised, mlp, 'second shape')
    make_prediction(third_shape_noised, mlp, 'third shape')
    make_prediction(fourth_shape_noised, mlp, 'fourth shape')
    make_prediction(fifth_shape_noised, mlp, 'fifth shape')


if __name__ == '__main__':
    main()
