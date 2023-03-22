#include "tftpclientfile.h"
#include "baseudp.h"
#include <iostream>

TFtpClientFile::~TFtpClientFile() { delete udp_; }

bool TFtpClientFile::getFile(std::string const& local_filename,
                             std::string const& remote_filename,
                             Mode mode)
{
    if(mode == TFtp::BINARY)
        write_file.open(local_filename.c_str(),
                        std::ifstream::out | std::ifstream::binary);
    else
        write_file.open(local_filename.c_str());

    if(!write_file.is_open())
        return false;

    read_req(remote_filename, mode);
    type_ = Write;
    return true;
}

bool TFtpClientFile::putFile(std::string const& local_filename,
                             std::string const& remote_filename,
                             Mode mode)
{
    if(mode == TFtp::BINARY)
        read_file.open(local_filename.c_str(),
                       std::ifstream::in | std::ifstream::binary);
    else
        read_file.open(local_filename.c_str());

    if(!read_file.is_open())
        return false;

    filesize_ = get_filesize(local_filename.c_str());
    write_req(remote_filename, mode);
    type_ = Read;
    return true;
}

void TFtpClientFile::on_data(uint16_t block_number, uint8_t const* data, uint32_t size)
{
    if(type_ != Write)
    {
        error(IllegalOperation, "Illegal TFTP Operation in Data");
        return;
    }
    write_file.write((char *)data, size);
    file_bytes_ += size;
    ack(block_number);
    if(size < BLOCK_SIZE)
    {
        filesize_ = file_bytes_;
        finished();
        write_file.close();
    }
}

void TFtpClientFile::on_ack(uint16_t block_number)
{
    if(type_ != Read)
    {
        error(IllegalOperation, "Illegal TFTP Operation in ACK");
        return;
    }
    if(read_file.eof())
    {
        std::cout << "send data is finished" << std::endl;
        finished();
        return;
    }
    if(block_number_ != block_number)
        resend();
    else
    {
        block_number_++;
        send_data(block_number_);
    }
}

void TFtpClientFile::on_error(uint16_t error, std::string const& error_msg)
{
     set_error((Error)error, error_msg + std::string(" come from remote"));
     finished();
}

void TFtpClientFile::send_data(uint16_t block_number)
{
    char* d = data();

    read_file.read(d, TFtp::BLOCK_SIZE);
    file_bytes_ += read_file.gcount();
    send(block_number, read_file.gcount());
}

uint32_t TFtpClientFile::write(uint8_t const *data, size_t size)
{
    return udp_->write((const char*)data, size);
}
