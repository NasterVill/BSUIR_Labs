from tkinter import filedialog, Label, Button, messagebox, simpledialog
from PIL import ImageTk

import Controller
from utils import resize_image, get_project_root


class View:
    _root = None

    _initial_image = None
    _modified_image = None

    _label_initial_image = None
    _label_modified_image = None

    def __init__(self, controller: Controller):
        self._controller = controller
        self._controller.set_view(self)
        self._root = self._controller.get_root()

    def configure_layout(self):
        self._controller.get_root()

        self._label_initial_image = Label(self._root, image=None, width=50, height=30)
        self._label_initial_image.grid(row=0, column=0, padx=5, pady=5)

        self._label_modified_image = Label(self._root, image=None, width=50, height=30)
        self._label_modified_image.grid(row=0, column=1, padx=5, pady=5)

        Label(self._root, text="Original").grid(row=1, column=0)
        Label(self._root, text="Modified").grid(row=1, column=1)

        Button(self._root, text="Choose an image", command=self.on_image_change)\
            .grid(row=2, column=0, columnspan=2, sticky='NSEW')

        Button(self._root, text="Filter (Sobel filter)", command=self._controller.sobel_filter_image)\
            .grid(row=3, column=0, columnspan=2, sticky='NSEW')

        Button(self._root, text="Image preparation (binary)", command=self.binarization)\
            .grid(row=4, column=0, columnspan=2, sticky='NSEW')

        Button(self._root, text="Image preparation (interval)", command=self.luminance_slice)\
            .grid(row=5, column=0, columnspan=2, sticky='NSEW')

        Button(self._root, text="Save modified image", command=self.on_save_image)\
            .grid(row=6, column=0, columnspan=2, sticky='NSEW')

        Button(self._root, text="Quit", command=self._controller.quit)\
            .grid(row=7, column=0, columnspan=2, sticky='NSEW')

    def update_ui(self, _initial_image, _new_image):
        initial_image = resize_image(_initial_image, (700, 700))
        new_image = resize_image(_new_image, (700, 700))

        self._initial_image = ImageTk.PhotoImage(initial_image, master=self._root)
        self._modified_image = ImageTk.PhotoImage(new_image, master=self._root)

        self._label_initial_image['image'] = self._initial_image
        self._label_modified_image['image'] = self._modified_image

        self._label_initial_image['width'] = 0
        self._label_initial_image['height'] = 0
        self._label_modified_image['width'] = 0
        self._label_modified_image['height'] = 0

        # messagebox.showinfo('Success!', 'Operation completed!')

    def on_image_change(self):
        filename = filedialog.askopenfilename(initialdir=get_project_root(), title="Select file",
                                              filetypes=(("JPEG files", "*.jpg"),))

        self._controller.on_image_change(filename)

    def on_save_image(self):
        if not self._controller.has_new_image():
            messagebox.showerror('Error', 'Couldn\'t save file!')

            return

        file = filedialog.asksaveasfile(mode='w', defaultextension=".png",
                                        filetypes=(("JPEG files", "*.jpg"),))
        if file:
            if self._controller.on_save_image(file.name):
                messagebox.showinfo('Success!', 'Image was saved by path {}'.format(file.name))

    def binarization(self):
        if not self._controller.has_new_image():
            messagebox.showerror('Error', 'Couldn\'t perform operation!')

            return

        threshold = simpledialog.askinteger("Input", "Binarization threshold",
                                             parent=self._root,
                                             minvalue=0, maxvalue=255)

        if threshold is not None:
            self._controller.binarization(threshold)

    def luminance_slice(self):
        if not self._controller.has_new_image():
            messagebox.showerror('Error', 'Couldn\'t perform operation!')

            return

        threshold = simpledialog.askinteger("Input", "Threshold",
                                            parent=self._root,
                                            minvalue=0, maxvalue=255)

        interval = simpledialog.askinteger("Input", "Interval",
                                            parent=self._root,
                                            minvalue=0, maxvalue=(255 - threshold))

        print(threshold, interval)

        if threshold is not None and interval is not None:
            self._controller.luminance_slice(threshold, threshold + interval)

    def start(self):
        self.configure_layout()

        self._root.mainloop()
