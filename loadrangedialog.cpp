#include "loadrangedialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include <algorithm>

LoadRangeDialog::LoadRangeDialog(int fileCount, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Fichiers à charger"));

    const int lastIndex = std::max(0, fileCount - 1);

    auto *info = new QLabel(tr("%1 fichiers trouvés (grain_0.txt à grain_%2.txt).")
                                .arg(fileCount).arg(lastIndex));

    spinFirst = new QSpinBox;
    spinFirst->setRange(0, lastIndex);
    spinFirst->setValue(0);

    spinLast = new QSpinBox;
    spinLast->setRange(0, lastIndex);
    spinLast->setValue(lastIndex);

    spinStep = new QSpinBox;
    spinStep->setRange(1, std::max(1, lastIndex));
    spinStep->setValue(1);

    summary = new QLabel;

    auto *form = new QFormLayout;
    form->addRow(tr("Premier fichier :"), spinFirst);
    form->addRow(tr("Dernier fichier :"), spinLast);
    form->addRow(tr("Pas :"), spinStep);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Charger"));
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(info);
    layout->addLayout(form);
    layout->addWidget(summary);
    layout->addWidget(buttons);

    // Le dernier fichier ne peut pas précéder le premier
    connect(spinFirst, &QSpinBox::valueChanged, this, [this](int first) {
        spinLast->setMinimum(first);
        updateSummary();
    });
    connect(spinLast, &QSpinBox::valueChanged, this, &LoadRangeDialog::updateSummary);
    connect(spinStep, &QSpinBox::valueChanged, this, &LoadRangeDialog::updateSummary);

    updateSummary();
}

std::vector<int> LoadRangeDialog::selectedFileNumbers() const
{
    std::vector<int> numbers;
    const int first = spinFirst->value();
    const int last = spinLast->value();
    const int step = spinStep->value();
    for (int n = first; n <= last; n += step)
        numbers.push_back(n);
    return numbers;
}

void LoadRangeDialog::updateSummary()
{
    const std::vector<int> numbers = selectedFileNumbers();
    summary->setText(tr("→ %1 fichiers seront chargés (grain_%2.txt à grain_%3.txt).")
                         .arg(numbers.size())
                         .arg(numbers.front())
                         .arg(numbers.back()));
}
