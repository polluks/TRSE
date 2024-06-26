#ifndef DIALOGCOLORS_H
#define DIALOGCOLORS_H

#include <QDialog>
#include "source/LeLib/limage/lcolorlist.h"
#include <QFileDialog>
#include <QDebug>
#include <QVector3D>
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/limageio.h"
namespace Ui {
class DialogColors;
}

class DialogColors : public QDialog
{
    Q_OBJECT

public:
    explicit DialogColors(QWidget *parent = nullptr);
    ~DialogColors();

    LColorList* m_lst;
    LColorList* m_org;
    LImage *m_img = nullptr;
    int m_curCol = 0;
    QString m_projectPath;
    void Initialize(LImage* img, LColorList* lc, QString path);

    void toGUI();
    void setColor(int cc,int type);

private slots:
    void on_pushButton_clicked();

    void on_cbmColors_activated(int index);

    void on_leRGB_textChanged(const QString &arg1);

    void on_leRGB4_textChanged(const QString &arg1);

    void on_btnLoad_clicked();

    void on_btnSave_2_clicked();

    void on_pushButton_2_clicked();

    void on_cbmBitplanes_currentIndexChanged(int index);


    void on_btnLoad_2_clicked();

private:
    Ui::DialogColors *ui;
};

#endif // DIALOGCOLORS_H
