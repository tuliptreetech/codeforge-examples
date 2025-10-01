#include "qpcpp.hpp"
#include <memory>
#include <cassert>
#include <vector>
#include <array>
#include "codeforge_qf_ctrl.hpp"

static std::array<QP::QEvt const *, 200> testQueueStorage;

enum : uint8_t {
    RECORDER_PRIORITY = 1,
    DUMMY_AO_A_PRIORITY,
    DUMMY_AO_B_PRIORITY,
    DUMMY_AO_C_PRIORITY,
    DUMMY_AO_D_PRIORITY,
    DUMMY_AO_E_PRIORITY,
    UNIT_UNDER_TEST_PRIORITY
};

enum Signals {
    GO_ASSERT = QP::Q_USER_SIG,
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

    void ReceiveData(uint8_t* data, size_t size) {
        if (size < 9) {
            return;
        }
        if (data[0] == 'C') {
            if (data[1] == 'o') {
                if (data[2] == 'd') {
                    if (data[3] == 'e') {
                        if (data[4] == 'F') {
                            if (data[5] == 'o') {
                                if (data[6] == 'r') {
                                    if (data[7] == 'g') {
                                        if (data[8] == 'e') {
                                            this->postLIFO(Q_NEW(QP::QEvt, GO_ASSERT));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(active);
    Q_STATE_DECL(be_assertive);
};

Example::Example()
    : QActive(&initial)
{
}

Q_STATE_DEF(Example, initial)
{
    (void) e; // unused parameter

    subscribe(GO_ASSERT);

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
    cms::test::qf_ctrl::Setup(MAX_SIGNAL, 1000, pubSubEventMemPoolConfigs);
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    underTest->start(
        cms::test::qf_ctrl::UNIT_UNDER_TEST_PRIORITY,
        testQueueStorage.data(),
        testQueueStorage.size(),
        nullptr,
        0U);
    cms::test::qf_ctrl::ProcessEvents();
    
    underTest->ReceiveData(const_cast<uint8_t*>(data), size);
    cms::test::qf_ctrl::Teardown();
    return 0;
}