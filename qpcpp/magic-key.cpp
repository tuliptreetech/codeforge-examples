#include "qpcpp.hpp"
#include <memory>
#include <cassert>
#include <vector>

struct ReceivedByte : QP::QEvt
{
    uint8_t data;
};


enum Signals {
    GO_ASSERT = QP::Q_USER_SIG,
    PROCESS_BYTE,
    MAX_SIGNAL
};

class Example final : public QP::QActive
{
public:
    Example();

    virtual ~Example() = default;

    Example(const Example &)            = delete;
    Example &operator=(const Example &) = delete;
    Example(Example &&)                 = delete;
    Example &operator=(Example &&)      = delete;

    void ReceiveByte(uint8_t data) {
        auto e  = Q_NEW(ReceivedByte, PROCESS_BYTE);
        e->data = data;
        PUBLISH(e, me);
    }

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(active);
    Q_STATE_DECL(be_assertive);

    std::vector<uint8_t> m_buffer;
    size_t m_index {0};
    std::vector<uint8_t> m_magicKey;
};

Example::Example()
    : QActive(&initial)
{
    m_magicKey = {'C', 'o', 'd', 'e', 'F', 'o', 'r', 'g', 'e'};
}

Q_STATE_DEF(Example, initial)
{
    (void) e; // unused parameter

    subscribe(GO_ASSERT);
    subscribe(PROCESS_BYTE);

    return tran(&active);
}

Q_STATE_DEF(Example, active)
{
    QP::QState status;
    switch (e->sig)
    {
        case GO_ASSERT: {
            status = tran(&be_assertive);
            break;
        }
        case PROCESS_BYTE: {
            auto event = static_cast<ReceivedByte const *>(e);
            m_buffer.push_back(static_cast<uint8_t>(event->data));

            if (m_buffer == m_magicKey) {
                status = tran(&be_assertive);
            } else {
                status = Q_RET_HANDLED;
            }

            // just ignore the byte
            status = Q_RET_IGNORED;
            break;
        }
        default: {
            status = super(&top);
            break;
        }
    }
    return status;
}

Q_STATE_DEF(Example, be_assertive)
{
    assert(!"be assertive triggered!");
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    for (size_t i = 0; i < size; ++i) {
        underTest->ReceiveByte(data[i]);
    }
    return 0;
}