#include "qpcpp.hpp"
#include <memory>
#include <cassert>
#include <vector>

enum Signals {
    GO_ASSERT = QP::Q_USER_SIG,
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
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    underTest->ReceiveData(const_cast<uint8_t*>(data), size);
    return 0;
}