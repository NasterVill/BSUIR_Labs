#include "FiltraringCommand.h"

#include <QProgressDialog>
#include <QApplication>

namespace Draw
{

FiltraringCommand::FiltraringCommand(QWidget *drawingArea, QImage *image, const int ratio):
    _drawingArea(drawingArea), _image(image), _ratio(ratio)
{
}

int FiltraringCommand::findMedian(int* Array, const int size)
{
    int i;
    int j;
    int median_index;
    int temp;
    int median;

    for (i = 1; i < size; i++)
    {
        j = i - 1;
        temp = Array[i];
        while (j >= 0 && temp < Array[j])
        {
            Array[j + 1] = Array[j];
            j--;
        }
        Array[j + 1] = temp;
    }

    median_index = (int)(size / 2);
    median = Array[median_index];

    return median;
}

void FiltraringCommand::filtrate()
{
    const int array_size = 9;

    int row = 0;
    int coloumn = 0;
    int size = 0;
    int row_limit = 3;
    int coloumn_limit = 3;
    int red_array[array_size];
    int green_array[array_size];
    int blue_array[array_size];

    for (int i = 0; i < this->_image->height(); i++)
    {
        for (int j = 0; j < this->_image->width(); j++)
        {
            row = i - 1;
            row_limit = 3;

            if (i == this->_image->height() - 1 || i == 0)
            {
                row_limit = 2;
                if(i == 0) row = i;
            }

            for (int y = row, k = 0; row_limit--; y++)
            {
                coloumn = j - 1;
                coloumn_limit = 3;

                if (j == this->_image->width() - 1 || j == 0)
                {
                    coloumn_limit = 2;
                    if (j == 0) coloumn = j;
                }

                for (int x = coloumn; coloumn_limit--; x++)
                {
                    red_array[k] = qRed(this->_image->pixel(x, y));
                    green_array[k] = qGreen(this->_image->pixel(x, y));
                    blue_array[k] = qBlue(this->_image->pixel(x, y));

                    k++;
                    size = k;
                }
            }

            int r = findMedian(red_array, size);
            int g = findMedian(green_array, size);
            int b = findMedian(blue_array, size);

            QRgb pixelColor = qRgb(r, g ,b);

            this->_image->setPixel(j, i, pixelColor);

            memset(red_array, 0, array_size * sizeof(int));
            memset(green_array, 0, array_size * sizeof(int));
            memset(blue_array, 0, array_size * sizeof(int));

            size = 0;
        }
    }
}

void FiltraringCommand::undo()
{
    *this->_image = this->_undoImage.copy();
    this->_drawingArea->update();
}

void FiltraringCommand::redo()
{
    this->_undoImage = this->_image->copy();

    QProgressDialog *progressDialog = new QProgressDialog("Filtrating...",
                                                          QString(),
                                                          0, this->_ratio,
                                                          this->_drawingArea,
                                                          Qt::WindowTitleHint);
    progressDialog->setWindowTitle("Please Wait");
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setMinimumHeight(70);
    progressDialog->setMinimumWidth(250);

    progressDialog->setValue(0);
    QApplication::processEvents();

    for(int i = 0; i < this->_ratio; i++)
    {
        progressDialog->setValue(i);
        QApplication::processEvents();
        filtrate();
    }
    progressDialog->setValue(this->_ratio);
    delete progressDialog;

    this->_drawingArea->update();
}


FiltraringCommand::~FiltraringCommand()
{
}

} // namespace Draw
