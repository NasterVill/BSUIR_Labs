from PIL import Image
import tkinter as tk
import numpy as np
from scipy import misc, ndimage

from utils import show_preconfigured_hist


class Controller:
    _root = None
    _initial_image: Image.Image = None
    _new_image: Image.Image = None
    _view = None

    def set_view(self, view):
        self._view = view

    def __init__(self, app_name):
        self._root = tk.Tk()
        self._root.wm_title(app_name)

    def quit(self):
        if not (self._initial_image is None):
            self._initial_image.close()

        # stops mainloop
        self._root.quit()
        # this is necessary on Windows to prevent fatal Python Error: PyEval_RestoreThread: NULL tstate
        self._root.destroy()

    def has_new_image(self):
        return self._new_image is not None

    def get_root(self):
        return self._root

    def on_image_change(self, file_name):
        self._initial_image = Image.open(file_name)
        self._new_image = self._initial_image.copy().convert("L")

        self._view.update_ui(self._initial_image, self._new_image)

    def on_save_image(self, filename):
        if self._new_image:
            if self._new_image.mode != 'RGB':
                self._new_image = self._new_image.convert('RGB')

            self._new_image.save(filename)

            return True

        return False

    def restore_original(self):
        self._new_image = self._initial_image.copy().convert('L')

    def binarization(self, threshold):
        self.restore_original()

        pixels = self._new_image.load()

        for i in range(self._new_image.size[0]):
            for j in range(self._new_image.size[1]):
                pixels[i, j] = 0 if pixels[i, j] <= threshold else 255

        show_preconfigured_hist(self._new_image.getdata())
        self._view.update_ui(self._initial_image, self._new_image)

    def luminance_slice(self, lower_threshold, upper_threshold):
        self.restore_original()

        pixels = self._new_image.load()  # create the pixel map

        for i in range(self._new_image.size[0]):
            for j in range(self._new_image.size[1]):
                pixels[i, j] = 0 if pixels[i, j] <= lower_threshold or pixels[i, j] >= upper_threshold else 255

        show_preconfigured_hist(self._new_image.getdata())
        self._view.update_ui(self._initial_image, self._new_image)

    def sobel_filter_image(self):
        self.restore_original()

        image = misc.fromimage(self._new_image)
        image = image.astype('int32')
        dx = ndimage.sobel(image, 1)  # horizontal derivative
        dy = ndimage.sobel(image, 0)  # vertical derivative
        mag = np.hypot(dx, dy)  # magnitude
        mag *= 255.0 / np.max(mag)  # normalize (Q&D)

        print(222222, mag)

        self._new_image = Image.fromarray(mag)

        show_preconfigured_hist(self._new_image.getdata())
        self._view.update_ui(self._initial_image, self._new_image)
