/**
 * File: DialogInputFileNameBase.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/22
*/

#include "DialogInputFileNameBase.h"

DialogInputFileNameBase::DialogInputFileNameBase(const QString& fileExtension, QWidget *parent): QDialog(parent)
{
    m_fileExtension = fileExtension;
}

DialogInputFileNameBase::~DialogInputFileNameBase() {}
