#include "predictioncollection.h"

#include <cmath>
#include <QChar>
#include <QVector>
#include <QStringList>
#include <QDebug>
#include <QSet>
#include <QRegularExpression>

namespace Sonetta {

namespace {

QString clean(const QString &str)
{
    QString cleaned;
    bool acceptWhitespace = false;

    for (QChar c : str)
    {
        c = c.toLower();
        if (c.isLetterOrNumber())
        {
            cleaned.append(c);
            acceptWhitespace = true;
        }
        else if (c.isSpace() && acceptWhitespace)
        {
            cleaned.append(' ');
            acceptWhitespace = false;
        }
    }

    return cleaned;
}

QVector<QString> cleanSplit(const QString &str)
{
    QVector<QString> parts;
    QString currentWord;

    for (QChar c : str)
    {
        c = c.toLower();
        if (c.isLetterOrNumber())
        {
            currentWord.append(c);
        }
        else if (!currentWord.isEmpty())
        {
            parts.append(currentWord);
            currentWord.clear();
        }
    }

    if (!currentWord.isEmpty())
        parts.append(currentWord);

    return parts;
}

QStringList removeDuplicates(const QStringList &strings)
{
    // Removes duplicates in a case-insensitve fashion

    QStringList matches;
    QSet<QString> existing;

    for (const QString &str : strings)
    {
        QString cleaned = clean(str);
        if (!existing.contains(cleaned))
        {
            matches.append(str);
            existing.insert(cleaned);
        }
    }

    return matches;
}

void simplify(QString &prediction)
{
    // Simplifies prediction matches by removing the following common
    // patterns found in tracks or artist names:

    // The following patterns cause the rest of the line beginning
    // with the pattern to be truncated:
    // feat. Artist
    // [ ... ]

    QRegularExpression regexp("\\sfeat\\.|\\s\\-\\s|\\[", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regexp.match(prediction);
    if (match.hasMatch())
    {
        prediction.truncate(match.capturedStart());
    }
}

qreal determineSubscore(const QString &part, const QString &match)
{
    // Assumes part and match are both clean
    qreal score = 0.0;

    for (int i = 0; i < match.length(); ++i)
    {
        if (part[0] == match[i])
        {
            int j = 0;
            qreal subscore = 0.0;
            int consecutive = 0;
            while (j < part.length() && j + i < match.length())
            {
                if (part[j] == match[i + j])
                {
                    // Letter matches
                    subscore += pow(2, consecutive);
                    ++consecutive;
                }
                else
                {
                    consecutive = 0;
                    subscore -= -2.0;
                }
                ++j;
            }

            score = qMax(subscore, score);
        }
    }

    return score;
}

qreal determineScore(const QString & partial, const QString & match)
{
    // Clean match
    QString cleaned = clean(match);

    // Split partial
    QVector<QString> parts = cleanSplit(partial);

    if (cleaned.isEmpty() || parts.isEmpty())
        return 0.0;

    // Now, for certain combinations of the individual parts,
    // generate a score, and keep only the highest score.

    qreal score = 0.0;

    for (int from = 0; from < parts.count(); ++from)
    {
        QString part;
        for (int to = from; to < parts.count(); ++to)
        {
            if (!part.isEmpty())
                part.append(' ');

            part.append(parts[to]);

            // Part contains the string we want to test against match
            qreal subscore = determineSubscore(part, cleaned);
            score = qMax(subscore, score);
        }
    }

    return score;
}
}

PredictionCollection::PredictionCollection(const QString &partial)
    :   m_partial(clean(partial))
{

}

void PredictionCollection::insert(const Spotinetta::Track &track)
{
    QString name = track.name();
    simplify(name);
    qreal popularity = (qreal) track.popularity() / 100;
    qreal score = determineScore(m_partial, name) * (0.75 + popularity / 2.0);
    insert(score, name);
}

void PredictionCollection::insert(const Spotinetta::Artist &artist)
{
    QString name = artist.name();
    simplify(name);
    qreal score = determineScore(m_partial, name) * 1.2;
    insert(score, name);
}

void PredictionCollection::insert(const Spotinetta::Album &album)
{
    QString name = album.name();
    simplify(name);
    qreal score = determineScore(m_partial, name) * 0.7;
    insert(score, name);
}

void PredictionCollection::insert(const Spotinetta::Playlist &playlist)
{
    QString name = playlist.name();
    simplify(name);
    qreal score = determineScore(m_partial, name) * 0.6;
    insert(score, name);
}

void PredictionCollection::insert(qreal score, const QString &match)
{
    m_predictions.insert(score, match);
}

QStringList PredictionCollection::predictions() const
{
    QStringList pred;

    for (auto i = m_predictions.cbegin(); i != m_predictions.cend(); ++i)
    {
        qreal score = i.key();
        QString match = i.value();
        if (score > 1e-2)
            pred.prepend(match);
        qDebug() << score << " : " << match;
    }

    pred = removeDuplicates(pred);

    return pred;
}





}
