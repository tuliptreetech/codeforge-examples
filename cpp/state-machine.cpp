#include <memory>
#include <cassert>
#include <vector>
#include <array>

enum ParsingState
{
    STATE_WAITING_FOR_C,
    STATE_WAITING_FOR_O,
    STATE_WAITING_FOR_D,
    STATE_WAITING_FOR_E,
    STATE_WAITING_FOR_F,
    STATE_WAITING_FOR_O2,
    STATE_WAITING_FOR_R,
    STATE_WAITING_FOR_G,
    STATE_WAITING_FOR_E2,
};

class Example final
{
public:
    Example();

    virtual ~Example() = default;

    Example(const Example &)            = delete;
    Example &operator=(const Example &) = delete;
    Example(Example &&)                 = delete;
    Example &operator=(Example &&)      = delete;

    void ReceiveData(uint8_t data) {
        switch (m_state) {
        case STATE_WAITING_FOR_C:
            if (data == 'C') {
                m_state = STATE_WAITING_FOR_O;
            }
            break;
        case STATE_WAITING_FOR_O:
            if (data == 'o') {
                m_state = STATE_WAITING_FOR_D;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_D:
            if (data == 'd') {
                m_state = STATE_WAITING_FOR_E;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_E:
            if (data == 'e') {
                m_state = STATE_WAITING_FOR_F;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_F:
            if (data == 'F') {
                m_state = STATE_WAITING_FOR_O2;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_O2:
            if (data == 'o') {
                m_state = STATE_WAITING_FOR_R;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_R:
            if (data == 'r') {
                m_state = STATE_WAITING_FOR_G;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_G:
            if (data == 'g') {
                m_state = STATE_WAITING_FOR_E2;
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        case STATE_WAITING_FOR_E2:
            if (data == 'e') {
                assert(!"being assertive");
            } else {
                m_state = STATE_WAITING_FOR_C;
            }
            break;
        }
    }

protected:
    ParsingState m_state = STATE_WAITING_FOR_C;
};

Example::Example()
{
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    for (size_t i = 0; i < size; ++i) {
        underTest->ReceiveData(data[i]);
    }

    return 0;
}
