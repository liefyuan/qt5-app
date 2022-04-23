#include "googlepinyin.h"
#include "pinyinime.h"
#include "qapplication.h"
#include "qdebug.h"

using namespace ime_pinyin;

static size_t fix_cand_len(size_t cand, int maxOutputLen)
{
    size_t i;
    size_t decode_len;
    size_t single = 0;
    size_t multi = 0;
    char16 *cand_buf = new char16[maxOutputLen];

    im_get_sps_str(&decode_len);
    if (decode_len == 1) {
        if (cand > 10) {
            cand = 10;
        }

        delete cand_buf;
        return cand;
    }

    for (i = 0; i < cand; i++) {
        im_get_candidate(i, cand_buf, maxOutputLen);
        if (strlen((char *)cand_buf) > 2) {
            multi++;
        } else {
            single++;
            if (single > 40) {
                break;
            }
        }
    }

    cand = multi + single;
    delete cand_buf;
    return cand;
}

GooglePinYin::GooglePinYin(QObject *parent) : QObject(parent)
{
    ok = false;
    maxInputLen = 26;
    maxOutputLen = 26;
}

bool GooglePinYin::open(const QString &dbPath)
{
    QString pyPath = QString("%1/dict_pinyin.dat").arg(dbPath);
    QString pyUserPath = QString("%1/dict_pinyin_user.dat").arg(dbPath);
    ok = im_open_decoder(pyPath.toUtf8().constData(), pyUserPath.toUtf8().constData());

    if (ok) {
        im_set_max_lens(maxInputLen, maxOutputLen);
        im_reset_search();
    } else {
        qDebug() << "load GooglePinYin error";
    }

    return ok;
}

void GooglePinYin::close()
{
    im_close_decoder();
}

void GooglePinYin::flush()
{
    im_flush_cache();
}

void GooglePinYin::reset()
{
    if (!ok) {
        return;
    }

    im_reset_search();
}

int GooglePinYin::select(const QString &pinyin)
{
    //最大支持26个字母
    if (!ok || pinyin.length() > 26) {
        return 0;
    }

    QByteArray bytearray = pinyin.toUtf8();
    char *py = bytearray.data();
    size_t count = im_search(py, bytearray.size());
    count = fix_cand_len(count, maxOutputLen);
    return count;
}

QString GooglePinYin::getChinese(int index)
{
    char16 *cand_buf = new char16[maxOutputLen];
    char16 *cand;
    QString cand_str;

    cand = im_get_candidate(index, cand_buf, maxOutputLen);
    if (cand) {
        cand_str = QString::fromUtf16(cand);
        if (index == 0) {
            cand_str.remove(0, im_get_fixed_len());
        }
    } else {
        cand_str = "";
    }

    delete cand_buf;
    return cand_str;
}

int GooglePinYin::deleteSelect(int pos)
{
    if (!ok) {
        return 0;
    }

    size_t count = im_delsearch(pos, false, false);
    count = fix_cand_len(count, maxOutputLen);
    return count;
}

int GooglePinYin::getPosition()
{
    const uint16 *start_pos;
    size_t pos_len;
    size_t fixed_len = im_get_fixed_len();

    pos_len = im_get_spl_start_pos(start_pos);
    if (fixed_len <= pos_len) {
        return (start_pos[fixed_len]);
    }

    return  0;
}
