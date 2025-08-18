#ifndef UDP_HPP
#define UDP_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

#include <cstdint>
#include <cstring>
#include <mutex>
#include <chrono>
#include <string>
#include <iostream>

class BusInterface;

struct UdpOptions
{
    std::string listen_ip{"0.0.0.0"};
    uint16_t listen_port{14553};
    std::string target_ip{""}; // boş ise: ilk gelenin adresini 'son gönderici' olarak kullan
    uint16_t target_port{14554};

    // socket opsiyonları
    bool reuse_addr{true};
    bool reuse_port{false}; // Linux’ta mevcut; aynı portu birden çok süreç paylaşabilir
    bool broadcast{true};
    int rcv_buf{0}; // 0 → değişme; >0 ise setsockopt(SO_RCVBUF)
    int snd_buf{0}; // 0 → değişme; >0 ise setsockopt(SO_SNDBUF)
    int tos{-1};    // -1 → değişme; yoksa IP_TOS
    int ttl{-1};    // -1 → değişme; yoksa IP_TTL
};

class Udp : public BusInterface
{
public:
    Udp(const std::string &listen_ip = "0.0.0.0",
        int listen_port = 14553,
        int target_port = 14554);

    explicit Udp(const UdpOptions &opts);
    ~Udp();

    Udp(const Udp &) = delete;
    Udp &operator=(const Udp &) = delete;

    bool connect() override;
    void disconnect() override;
    bool init() override;

    int write(const uint8_t *buffer, size_t size) override;
    int writeByte(const uint8_t *byte) override;
    int read(uint8_t *buffer, size_t size) override;
    int readByte(uint8_t *byte) override;

    void getConnectionInfo() const override;

    void setTargetAddress(const std::string &ip, int port);
    void setListenAddress(const std::string &ip, int port);
    std::string getTargetAddress() const;
    std::string getListenAddress() const;

    bool setConfig(const UdpOptions &opts); 
    UdpOptions getConfig() const;

private:
    bool setupSocketOptions();
    bool bindSocket();
    void setupPoll();
    bool isValidIpAddress(const std::string &ip) const;
    static bool isValidPort(int port);

    int waitReadable(int timeout_ms);

private:
    UdpOptions _options;

    pollfd _pollfds[1]{};
    sockaddr_in _listen_addr{};
    sockaddr_in _target_addr{};

    mutable std::mutex _mtx;
    std::chrono::steady_clock::time_point _timestamp;
};

#endif