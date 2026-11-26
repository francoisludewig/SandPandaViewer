#include "exportvideodialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include <algorithm>

ExportVideoDialog::ExportVideoDialog(int frameCount, const std::vector<int> &fileNumbers,
                                     QWidget *parent)
    : QDialog(parent), numbers(fileNumbers)
{
    setWindowTitle(tr("Exporter une vidéo"));

    const int lastIndex = std::max(0, frameCount - 1);

    auto *info = new QLabel(tr("%1 images chargées (grain_%2 à grain_%3).")
                                .arg(frameCount)
                                .arg(fileNumber(0))
                                .arg(fileNumber(lastIndex)));

    spinFps = new QSpinBox;
    spinFps->setRange(1, 120);
    spinFps->setValue(25);
    spinFps->setSuffix(tr(" img/s"));

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
    form->addRow(tr("Images par seconde :"), spinFps);
    form->addRow(tr("Image de départ :"), spinFirst);
    form->addRow(tr("Image de fin :"), spinLast);
    form->addRow(tr("Pas entre les images :"), spinStep);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Exporter..."));
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(info);
    layout->addLayout(form);
    layout->addWidget(summary);
    layout->addWidget(buttons);

    // La dernière image ne peut pas précéder la première
    connect(spinFirst, &QSpinBox::valueChanged, this, [this](int first) {
        spinLast->setMinimum(first);
        updateSummary();
    });
    connect(spinLast, &QSpinBox::valueChanged, this, &ExportVideoDialog::updateSummary);
    connect(spinStep, &QSpinBox::valueChanged, this, &ExportVideoDialog::updateSummary);
    connect(spinFps, &QSpinBox::valueChanged, this, &ExportVideoDialog::updateSummary);

    updateSummary();
}

int ExportVideoDialog::fps() const { return spinFps->value(); }
int ExportVideoDialog::first() const { return spinFirst->value(); }
int ExportVideoDialog::last() const { return spinLast->value(); }
int ExportVideoDialog::step() const { return spinStep->value(); }

int ExportVideoDialog::imageCount() const
{
    return (last() - first()) / step() + 1;
}

int ExportVideoDialog::fileNumber(int index) const
{
    if (index >= 0 && index < static_cast<int>(numbers.size()))
        return numbers[index];
    return index;
}

void ExportVideoDialog::updateSummary()
{
    const int count = imageCount();
    const int lastExported = first() + (count - 1) * step();
    const double duration = static_cast<double>(count) / fps();
    summary->setText(tr("→ %1 images (grain_%2 à grain_%3), durée %4 s.")
                         .arg(count)
                         .arg(fileNumber(first()))
                         .arg(fileNumber(lastExported))
                         .arg(duration, 0, 'f', 1));
}
