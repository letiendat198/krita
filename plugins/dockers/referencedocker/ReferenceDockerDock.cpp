#include "ReferenceDockerDock.h"
#include "KisDocument.h"
#include "KisViewManager.h"
#include "KoIcon.h"
#include <kis_canvas2.h>
#include <qgraphicsitem.h>

ReferenceDockerDock::ReferenceDockerDock()
    : QDockWidget(i18n("Reference Docker"))
    , m_ui(new Ui_WdgReferenceDocker())
{
    QWidget *mainWidget = new QWidget(this);
    m_ui->setupUi(mainWidget);

    mainWidget->setContentsMargins(5,10,5,10);

    m_ui->btnNext->setIcon(koIcon("arrow-right"));
    connect(m_ui->btnNext, &QToolButton::clicked, this, [this]() {
        this->changeCurrentImage(m_index+1);
    });

    m_ui->btnPrev->setIcon(koIcon("arrow-left"));
    connect(m_ui->btnPrev, &QToolButton::clicked, this, [this]() {
        this->changeCurrentImage(m_index-1);
    });

    m_ui->btnReset->setIcon(koIcon("zoom-fit"));
    connect(m_ui->btnReset, &QToolButton::clicked, m_ui->referenceView, &ReferenceViewWidget::resetView);

    m_ui->btnSample->setIcon(koIcon("krita_tool_color_sampler"));
    m_ui->btnSample->setCheckable(true);

    m_ui->sliderZoom->setMinimum(100);
    m_ui->sliderZoom->setMaximum(300);
    m_ui->sliderZoom->setSingleStep(20);
    connect(m_ui->sliderZoom, &QSlider::valueChanged, this, &ReferenceDockerDock::slotZoomSliderChanged);

    connect(m_ui->referenceView, &ReferenceViewWidget::scaleChanged, this, &ReferenceDockerDock::slotViewScaleChanged);

    setWidget(mainWidget);
}

void ReferenceDockerDock::referenceImageChanged() {
    if (!m_document) return;
    // dbgUI<<"Reference image changed";
    KisReferenceImagesLayerSP layer = m_document->referenceImagesLayer();

    if (!layer || layer->referenceImages().size() == 0) {
        m_ui->referenceView->clearView();
        m_index = 0;
        return;
    }
    changeCurrentImage(m_index);
}

void ReferenceDockerDock::slotZoomSliderChanged(int value) {
    m_ui->lblZoomPercent->setText(QString::number(value) + "%");

    qreal factor = (qreal)value / (qreal)100;
    const QSignalBlocker blocker(m_ui->referenceView);
    m_ui->referenceView->setScale(factor);
}

void ReferenceDockerDock::slotViewScaleChanged(qreal factor) {
    int value = factor * 100;
    m_ui->lblZoomPercent->setText(QString::number(value) + "%");
    // In case this function isn't called by slider signal
    m_ui->sliderZoom->setValue(value);
}

void ReferenceDockerDock::changeCurrentImage(int index) {
    if (!m_document) return;

    KisReferenceImagesLayerSP layer = m_document->referenceImagesLayer();
    if (layer) {
        if (index >= layer->referenceImages().size() || index < 0) {
            return;
        }

        m_ui->referenceView->setReferenceImage(layer->referenceImages().at(index));

        m_index = index;
    }
}

void ReferenceDockerDock::setViewManager(KisViewManager* vm) {
    if (!vm) return;

    if (vm != m_vm) {
        m_vm = vm;
        connect(vm, &KisViewManager::viewChanged, this, &ReferenceDockerDock::slotViewChanged);
    }
}

void ReferenceDockerDock::slotViewChanged() {
    if (!m_vm) return;

    KisDocument *document = m_vm->document();

    if (document && document != m_document) {
        if (m_document) m_document->disconnect(this);
        m_document = document;

        // We only keep one index so should reset it when view change. Stack?
        m_index = 0;

        // Handle adding the first reference or removing the last reference
        // Anything in between don't really matter

        // This signal will fire when add/delete/move ref image
        // But when delete last ref, it fire before delete?
        connect(document, &KisDocument::sigReferenceImagesChanged, this, &ReferenceDockerDock::referenceImageChanged);

        // This signal will fire when ref layer get deleted (when ref image is 0)
        // But oddly won't fire when a ref layer is created
        // So I did both
        connect(document, &KisDocument::sigReferenceImagesLayerChanged, this, &ReferenceDockerDock::referenceImageChanged);

        // Force load when view change
        referenceImageChanged();
    }
}

// TODO: Won't show opacity/saturation change in reference tool option
void ReferenceDockerDock::setCanvas(KoCanvasBase *canvas) {}
