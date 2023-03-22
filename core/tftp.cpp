#include "tftp.h"
#include <vector>
#include <sys/stat.h>

#define MIN_PACKET_LEN 4

#define WITE_CODE(data, code) \
    data[0] = uint8_t(code >> 8); \
    data[1] = uint8_t(code >> 0);

#define WITE_HEAD(data, code, value) \
    data[0] = uint8_t(code >> 8); \
    data[1] = uint8_t(code >> 0); \
    data[2] = uint8_t(value >> 8);\
    data[3] = uint8_t(value >> 0);

TFtp::TFtp()
{
}

bool TFtp::process(uint8_t const *data, uint32_t size)
{
    if(size < MIN_PACKET_LEN)
        return false;

    uint16_t code = op_code(data);
    if(code == RRQ || code == WRQ)
    {
        uint8_t const*d = data + sizeof(uint16_t);
        uint8_t const*e = data + size;
        uint8_t const*s = d;

        while(s < e && *s)
            s++;
        std::string filename((char *)d, s - d);

        s++;
        d = s;
        while(s < e && *s)
            s++;
        std::string mode_text((char *)d, s - d);

        if(code == RRQ)
            on_read_req(filename, getMode(mode_text));
        else
            on_write_req(filename, getMode(mode_text));
        return true;
    }
    else if(code == DATA)
    {
        on_data(block_num(data), &data[HEADER_SIZE], size - HEADER_SIZE);
        return true;
    }
    else if(code == ACK)
    {
        on_ack(block_num(data));
        return true;
    }
    else if(code == ERROR)
    {
        uint8_t const* d = data + HEADER_SIZE;
        uint8_t const *e = data + size;
        uint8_t const *s = d;

        while(s < e && *s)
            s++;

        on_error(error_code(data), std::string((char *)d, s - d));
        return true;
    }
    return false;
}

void TFtp::on_read_req(std::string const& , Mode)
{
}

void TFtp::on_write_req(std::string const&, Mode)
{
}

void TFtp::read_req(std::string const& filename, Mode mode)
{
    std::string text = getText(mode);
    std::vector<uint8_t> data(CODE_SIZE + filename.size() + text.size() + 2, 0);

    WITE_CODE(data, RRQ)
    memcpy(&data[CODE_SIZE], filename.c_str(), filename.size());
    memcpy(&data[CODE_SIZE + filename.size() + 1], text.c_str(), text.size());

    write(&data[0], data.size());
}

void TFtp::write_req(std::string const& filename, Mode mode)
{
    std::string text = getText(mode);
    std::vector<uint8_t> data(CODE_SIZE + filename.size() + text.size() + 2, 0);

    WITE_CODE(data, WRQ)
    memcpy(&data[CODE_SIZE], filename.c_str(), filename.size());
    memcpy(&data[CODE_SIZE + filename.size() + 1], text.c_str(), text.size());

    write(&data[0], data.size());
}

void TFtp::send(uint16_t block_number, size_t size)
{
   WITE_HEAD(data_, DATA, block_number)
   block_size_ = size;
   write(data_, size + HEADER_SIZE);
}

void TFtp::resend()
{
    write(data_, block_size_ + HEADER_SIZE);
}

void TFtp::ack(uint16_t block_number)
{
    std::vector<uint8_t> data(HEADER_SIZE);
    WITE_HEAD(data, ACK, block_number)
    write(&data[0], data.size());
}

void TFtp::error(Error error, std::string const& error_msg)
{
    std::vector<uint8_t> data(HEADER_SIZE + error_msg.size() + 1);
    error_ = error;
    error_msg_ = error_msg;
    finished();

    WITE_HEAD(data, ERROR, error)
    memcpy(&data[HEADER_SIZE], error_msg.c_str(), error_msg.size());
    data[data.size() - 1] = 0;

    write(&data[0], data.size());
}

TFtp::Mode TFtp::getMode(std::string const& text)
{
    if(text == "octet")
        return BINARY;
    else if(text == "netascii")
        return ASCII;
    else
        return MAIL;
}

std::string TFtp::getText(TFtp::Mode mode)
{
    if(mode == BINARY)
        return std::string("octet");
    else if(mode == ASCII)
        return std::string("netascii");
   return std::string("mail");
}

size_t TFtp::get_filesize(const char*filename)
{
    if(!filename)
        return 0;
    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_size;

}
