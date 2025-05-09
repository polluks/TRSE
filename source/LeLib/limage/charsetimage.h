/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CHARSETIMAGE_H
#define CHARSETIMAGE_H

#include "multicolorimage.h"
#include "source/LeLib/util/util.h"
#include <QPixmap>


class CharsetImage : public MultiColorImage
{
public:
    QByteArray m_rawData;
    PixelChar m_color;
    CharsetImage(LColorList::Type t);

    int m_skipImportBytes = 0;
//    PixelChar m_copy[64];

    int m_colorOrderType = 0; // C64

    int m_displayGridKeep;

    int FindClosestChar(PixelChar p);


    QString GetCurrentDataString() override;


    virtual QString getMetaInfo() override;


    void SetColor(uchar col, uchar idx) override;

    void SaveBin(QFile& file) override;
    void LoadBin(QFile& file) override;

    void SavePensBin(QFile& file);
    void LoadPensBin(QFile& file);

    uchar getVariableColor(PixelChar* pc);


    void LoadCharset(QString file, int skipBytes) override;


    LImage* getCharset() override { return this; }
    unsigned int getPixel(int x, int y) override;

    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;
    virtual void FromRaw(QByteArray& arr);
    virtual void ToRaw(QByteArray& arr);
    virtual QPixmap ToQPixMap(int chr) override;
    virtual void setPixel(int x, int y, unsigned int color) override;

    virtual void CreateMagicalCharset() override;




    void SetBank(int bnk) override;
    QPoint getXY(int x, int y);

    virtual unsigned int getCharPixel(int pos, int pal, int x, int y);

    void Invert() override;


    void RenderEffect(QHash<QString, float> params) override;

//    void CopyFrom(LImage* mc) override;
    bool KeyPress(QKeyEvent *e) override;

    void setLimitedPixel(int x, int y, unsigned int color);

    void onFocus() override;

    virtual int getCharWidthDisplay() override;
    virtual int getCharHeightDisplay() override;
    virtual int getGridWidth() override;


    virtual int charWidthDisplay() override;

    virtual int getDx() override;
    virtual int getDy() override;


    void ExportFrame(QFile &file, int frame, int frameCount, int type, int col, int row, int width, int height, int rowOrder) override;

    unsigned int getPixelHybrid(int x, int y);

    void setPixelHybrid(int x, int y, unsigned int color);

    void setBackground(unsigned int col) override;

//    virtual void InitPens() override;


/*    void FlipHorizontal() override;
    void FlipVertical() override;

    void CopyChar() override;
    void PasteChar() override;
*/
};

#endif // CHARSETIMAGE_H
