#pragma once

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QQmlPropertyMap>

namespace Sonetta {
/**
 * @brief Facilitates coordination of UI states and visual style information with the QML frontend.
 */

class UIStateCoordinator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool hasNextState READ hasNextState NOTIFY stateChanged)
    Q_PROPERTY(bool hasPreviousState READ hasPreviousState NOTIFY stateChanged)

    // The following properties are property maps
    Q_PROPERTY(QObject * fonts READ fonts NOTIFY fontsChanged)
    Q_PROPERTY(QObject * colors READ colors NOTIFY colorsChanged)
    Q_PROPERTY(QObject * misc READ misc NOTIFY miscChanged)

    Q_ENUMS(Direction)

public:
    enum Direction {
        Forward,
        Backward,
        None
    };

    explicit UIStateCoordinator(QObject *parent = 0);

    QVariant state() const;

    Q_INVOKABLE void resetState(const QVariant &state, Direction direction = Backward);
    Q_INVOKABLE void pushState(const QVariant &state);
    Q_INVOKABLE void initState(const QVariant &state);

    bool hasNextState() const;
    bool hasPreviousState() const;

    Q_INVOKABLE void setFontProperties(const QVariant &fonts);
    Q_INVOKABLE void setColorProperties(const QVariant &colors);
    Q_INVOKABLE void setMiscProperties(const QVariant &misc);

    QObject * fonts() const;
    QObject * colors() const;
    QObject * misc() const;

public slots:
    void previousState();
    void nextState();
    
signals:
    void stateChanged(Direction direction);
    void fontsChanged();
    void colorsChanged();
    void miscChanged();
    
private:
    typedef QVector<QVariant> StateStack;

    StateStack  m_states;
    int         m_currentStateIndex;

    QQmlPropertyMap * m_fonts;
    QQmlPropertyMap * m_colors;
    QQmlPropertyMap * m_misc;
};

}
