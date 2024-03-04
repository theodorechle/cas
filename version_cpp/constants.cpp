#include "constants.hpp"

namespace constants {

std::ostream& operator<<(std::ostream& o, const Types type) {
    std::string value;
    switch (type) {
        case Types::NUL: value = "NUL";break;
        case Types::VAR: value = "VAR";break;
        case Types::NBR: value = "NBR";break;
        case Types::OPA: value = "OPA";break;
        case Types::CPA: value = "CPA";break;
        case Types::FUC: value = "FUC";break;
    }
    o << value;
    return o;
}

}