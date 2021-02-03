// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <QObject>

namespace metier
{
class QmlInterface final : public QObject
{
    Q_OBJECT

signals:
    void displayBlockchainChooser();
    void displayFirstRun();
    void displayMainWindow();
    void displayNamePrompt();

public:
    auto doDisplayBlockchainChooser() -> void;
    auto doDisplayFirstRun() -> void;
    auto doDisplayMainWindow() -> void;
    auto doDisplayNamePrompt() -> void;

    ~QmlInterface() final;
};
}  // namespace metier
