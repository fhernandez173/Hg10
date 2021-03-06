/*
 * ListContactsController.hpp
 *
 *  Created on: 12 oct. 2014
 *      Author: pierre
 */

#ifndef LISTCONTACTSCONTROLLER_HPP_
#define LISTCONTACTSCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <bb/platform/Notification>
#include <bb/cascades/ActivityIndicator>
#include <bb/system/SystemUiResult>
#include <QSet>

class Contact;


class ListContactsController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString userName    READ getUserName    WRITE setUserName    NOTIFY userNameChanged)
    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)
    Q_PROPERTY( QString presence    READ getPresence    WRITE setPresence    NOTIFY presenceChanged)
    Q_PROPERTY( int     available   READ isAvailable    WRITE setAvailable   NOTIFY availableChanged)
    Q_PROPERTY( int     priority    READ getPriority    WRITE setPriority    NOTIFY priorityChanged)
    Q_PROPERTY( bool choosePriority READ isChoosePriority WRITE setChoosePriority NOTIFY choosePriorityChanged)

    Q_PROPERTY( bool    showOnlyFav READ isOnlyFav                           NOTIFY showOnlyFavChanged)
    Q_PROPERTY( int     availabilityFilter READ getAvailabilityFilter        NOTIFY availabilityFilterChanged)
    Q_PROPERTY( int     contactSortingKey  READ getContactSortingKey         NOTIFY contactSortingKeyChanged)


    Q_PROPERTY( int     conversTheme READ getConversTheme                    NOTIFY  conversThemeChanged)

    Q_PROPERTY( QString firstName    READ getFirstName    WRITE setFirstName    NOTIFY firstNameChanged)
    Q_PROPERTY( QString lastName     READ getLastName     WRITE setLastName     NOTIFY lastNameChanged)
    Q_PROPERTY( QString nickname     READ getNickname     WRITE setNickname     NOTIFY nicknameChanged)

private:
    bb::cascades::ListView          *m_ListView;
    bb::cascades::ListView          *m_BlackListView;
    bb::cascades::ActivityIndicator *m_Activity;
    QString                          m_User;
    QString                          m_Avatar;
    QString                          m_Presence;
    QString                          m_FirstName;
    QString                          m_LastName;
    QString                          m_Nickname;
    int                              m_Available;
    int                              m_Priority;
    bool                             m_ChoosePriority;
    QList<Contact *>                 m_Contacts;
    bool                             m_OnlyFavorite;
    int                              m_AvailabilityFilter;
    int                              m_ContactSortingKey;
    bool                             m_PushStated;
    int                              m_ConversTheme;


    bb::platform::Notification      *m_Notification;

    QSet<QString>                    m_BlackList;
    QString                          m_tmp_blacklist;


public:
    ListContactsController              (QObject *parent = 0);
    virtual ~ListContactsController     ()                      {};

    inline const QString &getUserName   () const               { return m_User; }
    inline void           setUserName   (const QString &c)     { m_User = c; emit userNameChanged(); }


    inline const QString &getAvatar     () const               { return m_Avatar; }
    inline void           setAvatar     (const QString &c)     { m_Avatar = c; emit avatarChanged(); }

    inline const QString &getPresence   () const               { return m_Presence; }
    inline void           setPresence   (const QString &c)     { m_Presence = c; emit presenceChanged(); }

    inline const QString &getFirstName   () const               { return m_FirstName; }
    inline void           setFirstName   (const QString &c)     { m_FirstName = c; emit firstNameChanged(); }

    inline const QString &getLastName   () const               { return m_LastName; }
    inline void           setLastName   (const QString &c)     { m_LastName = c; emit lastNameChanged(); }

    inline const QString &getNickname   () const               { return m_Nickname; }
    inline void           setNickname   (const QString &c)     { m_Nickname = c; emit nicknameChanged(); }

    inline bool           isAvailable   () const               { return m_Available; }
    inline void           setAvailable  (bool c)               { m_Available = c; emit availableChanged(); }

    inline bool           isOnlyFav     () const               { return m_OnlyFavorite; }
    inline int            getAvailabilityFilter() const        { return m_AvailabilityFilter; }
    inline int            getContactSortingKey() const         { return m_ContactSortingKey; }

    inline int            getConversTheme() const           { return m_ConversTheme; }

    inline bool           isChoosePriority() const             { return m_ChoosePriority; }
    inline void           setChoosePriority(bool c)            {if(c != m_ChoosePriority) {  m_ChoosePriority = c; emit choosePriorityChanged(); } }

    inline int            getPriority   () const               { return m_Priority; }
    inline void           setPriority  (bool c)                {if(c != m_Priority) {  m_Priority = c; emit priorityChanged(); } }

    void                  saveBlackList();
    void                  loadBlackList();


public Q_SLOTS:
    inline void setListView             (QObject *listView)    {m_ListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    inline void setBlackListView        (QObject *listView)    {m_BlackListView = dynamic_cast<bb::cascades::ListView*>(listView); }
    inline void setActivityIndicator    (QObject *activity)    {m_Activity = dynamic_cast<bb::cascades::ActivityIndicator*>(activity); m_Activity->start();}
    void updateView                     ();
    void pushContact                    (const Contact*);
    void refresh                        ();

    void updateBlackList                ();

    void loadContactDetails             (const QString &id);
    void blacklistContact               (const QString &id);
    void removeFromBlackList            (const QString &id);
    void onPromptFinishedBlacklist      (bb::system::SystemUiResult::Type);


    void updatePresence                 (const QString &who, int status);
    void messageReceived                (const QString &from, const QString &message);
    void markRead                       ();
    void markAllRead                    ();

    QString formatTime                  (qint64 msecs);

    void filter                         (const QString &contacts);
    void setFilter                      (bool onlyFav);
    void setAvailabilityFilter          (int level);
    void setContactSortingKey           (int index);
    void updateConnectionStatus         (bool status);
    void selectFirst                    ();

    void editContact                    (const QString &id, const QString &fullname, const QString &firstname, const QString &lastname, const QString &nickname);

    void clear                          ();
    void deleteHistory                  (const QString &with);

    void setPresence                    (const QString &text, int presence, bool setPriority, int priority);


    void addContact                     ();
    void onPromptFinishedAddContact     (bb::system::SystemUiResult::Type);


Q_SIGNALS:
    void complete                       ();
    void cleared                        ();

    void userNameChanged                ();
    void avatarChanged                  ();
    void conversThemeChanged            ();
    void priorityChanged                ();
    void choosePriorityChanged          ();

    void firstNameChanged               ();
    void lastNameChanged                ();
    void nicknameChanged                ();

    void presenceChanged                ();
    void availableChanged               ();
    void showOnlyFavChanged             ();
    void availabilityFilterChanged      ();
    void contactSortingKeyChanged       ();

    void userSelected                   (const QString &id, const QString &name, const QString &avatar);


};



#endif /* LISTCONTACTSCONTROLLER_HPP_ */
