from View import View
from Controller import Controller


def main():
    controller = Controller("Image Processing")
    view = View(controller)
    view.start()


if __name__ == '__main__':
    main()
