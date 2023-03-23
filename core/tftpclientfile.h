#ifndef TFTPCLIENTFILE_H
#define TFTPCLIENTFILE_H
#include "tftp.h"
#include <memory>
#include <fstream>

class BaseUdp;
class TFtpClientFile : public TFtp
{
public:
    TFtpClientFile(BaseUdp *udp)
        : udp_(udp)
        , type_(None)
    {}

    ~TFtpClientFile();

    bool getFile(std::string const& local_filename,
                 std::string const& remote_filename, Mode mode);
    bool putFile(std::string const& local_filename,
                 std::string const& remote_filename, Mode mode);

    size_t filesize() const { return filesize_; }
    size_t file_bytes() const { return file_bytes_; }

    using Ptr = std::shared_ptr<TFtpClientFile>;
protected:
    void on_data(uint16_t block_number, uint8_t const* data, uint32_t size) override;
    void on_ack(uint16_t block_number) override;
    void on_error(uint16_t error, std::string const& error_msg) override;
    uint32_t write(uint8_t const *data, size_t size) override;
private:
    void send_data(uint16_t block_number);
private:
    BaseUdp* udp_;
    Type type_;
    std::ifstream read_file;
    std::ofstream write_file;
    uint16_t block_number_ = 0;
    uint32_t block_size_ = 0;
    size_t filesize_ = 0;
    size_t file_bytes_ = 0;
};

#endif // TFTPCLIENTFILE_H
