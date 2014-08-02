/**
 * File: IObserver.h
 * Developed by: Biancucci Maurizio
 * Date: 2014/07/08
*/

#ifndef IOBSERVER_H
#define IOBSERVER_H

/**
 * @brief The UPDATE_CODE enum
 * @param UPDATE_ALL to update all
 * @param UPDATE_LAST_ADD to upddate adding only the last modifies
 * @param UPDATE_ERR to stop and show the error that has occured
 */
enum UPDATE_CODE {UPDATE_ATTRIBUTE_NAME_TAG,
                  UPDATE_XML_DOCUMENT,
                  UPDATE_NEW_ASSOCIATED_FILE, UPDATE_REMOVED_ASSOCIATED_FILE,
                  UPDATE_XML_RELATIONS};

/**
 * @brief The Observer class implements the Observer Design pattern
 */
class IObserver
{
public:
    /**
     * @brief ~IObserver virtual destructor
     */
    virtual ~IObserver();

    /**
     * @brief Update
     * @param updateCode: to know what time of update is needed
     */
    virtual void Update(UPDATE_CODE updateCode) = 0;

};

#endif // IOBSERVER_H
