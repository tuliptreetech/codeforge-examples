#include "qpcpp.hpp"
#include <memory>
#include <cassert>
#include <vector>
#include <array>
#include "codeforge_qf_ctrl.hpp"

static std::array<QP::QEvt const *, 200> testQueueStorage;

enum : uint8_t {
    RECORDER_PRIORITY = 1,
    UNIT_UNDER_TEST_PRIORITY
};

struct ReceivedByte : QP::QEvt
{
    uint8_t data;
};


enum Signals {
    GO_ASSERT = QP::Q_USER_SIG,
    PROCESS_BYTE,
    MAX_SIGNAL
};

const cms::test::qf_ctrl::MemPoolConfigs pubSubEventMemPoolConfigs = {
    {sizeof(uint64_t) * 4, 500}, // small event pool
    {sizeof(uint64_t) * 16, 20}, // medium event pool
    {sizeof(uint64_t) * 250, 5}, // large event pool
                                 // FYI for some reason events can't be bigger than 1000 bytes
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
    cms::test::qf_ctrl::Setup(MAX_SIGNAL, 1000, pubSubEventMemPoolConfigs);
    underTest->start(
        cms::test::qf_ctrl::UNIT_UNDER_TEST_PRIORITY,
        testQueueStorage.data(),
        testQueueStorage.size(),
        nullptr,
        0U);
    cms::test::qf_ctrl::ProcessEvents();

    for (size_t i = 0; i < size; ++i) {
        underTest->ReceiveByte(data[i]);
    }

    cms::test::qf_ctrl::Teardown();
    return 0;
}