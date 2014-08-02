/**
 * File: Subject.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <QSet>

class IObserver;
enum UPDATE_CODE;

/**
 * @brief The Subject class that implements the subject-observer design pattern
 */
class Subject
{
private:
    /**
     * @brief observers: set of the attached observers
     */
    QSet<IObserver*> m_pObservers;

public:
    /**
     * @brief ~Subject virtual destructor
     */
    virtual ~Subject();

    /**
     * @brief AttachObserver: to attach an object that will be update every modification of this observable object
     * @param observer: the observer that will be updated
     */
    void AttachObserver(IObserver* p_observer);

    /**
     * @brief DetachObserver: to detach an Observer object previusly attached
     * @param p_observer: the observer that won't be updated any more
     */
    void DetachObserver(IObserver* p_observer);

    /**
     * @brief Notify: calls the update methods of all observer objects attached
     */
    void Notify(UPDATE_CODE updateCode);
};

#endif // IOBSERVABLE_H
