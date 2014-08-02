/**
 * File: DialoAssociatedFilesBase.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/18
*/

#include "DialogAssociatedFilesBase.h"

DialogAssociatedFilesBase::DialogAssociatedFilesBase(AssociatedFiles *associatedFiles, QWidget *parent):
    m_pAssociatedFiles(associatedFiles), QDialog(parent) {}

DialogAssociatedFilesBase::~DialogAssociatedFilesBase() {}
