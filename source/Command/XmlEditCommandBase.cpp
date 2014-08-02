/**
 * File: XmlEditCommand.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/28
*/

#include "XmlEditCommandBase.h"

XmlEditCommandBase::XmlEditCommandBase(XmlDocument* xmlDocument)
{
    m_pXmlDocument = xmlDocument;
}

XmlEditCommandBase::~XmlEditCommandBase() {}

XmlDocument* XmlEditCommandBase::GetXmlDocument() const
{
    return m_pXmlDocument;
}

QString XmlEditCommandBase::ComputeParentIdentifier(const QString& childIdentifier)
{
    QString parentIdentifier = "";

    //Compute the identifier of the parent
    int indexOfLastDot = childIdentifier.lastIndexOf(".");

    //If thers's no dot
    if(indexOfLastDot == -1)
    {
        //The parent is the root node
        parentIdentifier = "0";
    }
    else //Remove the last number and the point
    {
        parentIdentifier = childIdentifier;
        parentIdentifier.remove(indexOfLastDot, childIdentifier.size()-indexOfLastDot);
    }

    return parentIdentifier;
}
