/**
 * File: Subject.cpp
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#include "Subject.h"

#include "iobserver.h"

Subject::~Subject() {}

void Subject::AttachObserver(IObserver *p_observer)
{
    m_pObservers.insert(p_observer);
}

void Subject::DetachObserver(IObserver* p_observer)
{
    m_pObservers.remove(p_observer);
}

void Subject::Notify(UPDATE_CODE updateCode)
{
    QSetIterator<IObserver*> i(m_pObservers);
     while (i.hasNext())
     {
         i.next()->Update(updateCode);
     }
}
