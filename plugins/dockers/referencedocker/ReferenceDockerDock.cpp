#include "ReferenceDockerDock.h"
#include "KisDocument.h"
#include "KoIcon.h"
#include <kis_canvas2.h>
#include <qgraphicsitem.h>

ReferenceDockerDock::ReferenceDockerDock()
    : QDockWidget(i18n("Reference Docker"))
    , m_ui(new Ui_WdgReferenceDocker())
    , m_scene(new QGraphicsScene())
{
    QWidget *mainWidget = new QWidget(this);
    m_ui->setupUi(mainWidget);

    m_ui->referenceView->setScene(m_scene.data());
    m_ui->referenceView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->referenceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // QRect rContent = m_ui->referenceView->contentsRect();
    // m_ui->referenceView->setSceneRect(0, 0, rContent.width(), rContent.height());
    // m_ui->referenceView->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

    m_ui->referenceView->setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);

    m_ui->btnNext->setIcon(koIcon("arrow-right"));
    connect(m_ui->btnNext, &QToolButton::clicked, this, [this]() {
        this->changeCurrentImage(m_index+1);
    });

    m_ui->btnPrev->setIcon(koIcon("arrow-left"));
    connect(m_ui->btnPrev, &QToolButton::clicked, this, [this]() {
        this->changeCurrentImage(m_index-1);
    });

    m_ui->btnReset->setIcon(koIcon("zoom-fit"));
    connect(m_ui->btnReset, &QToolButton::clicked, this, &ReferenceDockerDock::fitImage);

    m_ui->sliderZoom->setMinimum(100);
    m_ui->sliderZoom->setMaximum(300);
    m_ui->sliderZoom->setSingleStep(20);
    connect(m_ui->sliderZoom, &QSlider::valueChanged, this, &ReferenceDockerDock::changeViewZoomFactor);

    setWidget(mainWidget);
}

void ReferenceDockerDock::loadDefaultOrClear() {
    KisReferenceImagesLayerSP layer = m_document->referenceImagesLayer();

    if (!layer) {
        m_scene->clear();
        return;
    }

    int refImageCount = layer->referenceImages().size();

    if (refImageCount == 0)  m_scene->clear();
    else {
        // If scene empty and have ref image. This is for when ref is first added
        if (m_scene->items().size() == 0) changeCurrentImage(0);
    }
}

void ReferenceDockerDock::changeViewZoomFactor(int value) {
    m_ui->lblZoomPercent->setText(QString::number(value) + "%");
    // In case this function isn't called by slider signal
    m_ui->sliderZoom->setValue(value);

    qreal factor = (qreal)value / (qreal)100;
    // dbgUI << "Zoom with factor: " << factor;

    qreal trueFactor = m_baseFactor + factor - 1;

    // Won't work if we have rotate
    m_ui->referenceView->setTransform(QTransform().scale(trueFactor, trueFactor), false);
}

void ReferenceDockerDock::changeCurrentImage(int index) {
    // dbgUI << "Change reference image to index: " << index;

    KisReferenceImagesLayerSP layer = m_document->referenceImagesLayer();
    if (layer) {
        if (index >= layer->referenceImages().size() || index < 0) {
            return;
        }

        m_scene->clear();
        QPixmap pixmap = QPixmap::fromImage(layer->referenceImages().at(index)->getImage());
        QGraphicsPixmapItem *item = m_scene->addPixmap(pixmap);
        item->setFlag(QGraphicsItem::ItemIsMovable, true);

        fitImage();

        m_index = index;
    }
}

void ReferenceDockerDock::fitImage() {
    if (m_scene->items().size()) {
        QGraphicsItem *item = m_scene->items().at(0);
        item->setPos(0, 0);
        m_ui->referenceView->fitInView(item, Qt::KeepAspectRatio);
        m_ui->referenceView->centerOn(item);

        m_baseFactor = m_ui->referenceView->transform().m11();

        changeViewZoomFactor(100);
    }
}

void ReferenceDockerDock::setViewManager(KisViewManager* vm) {
    if (!vm) return;

    // TODO: Handle view change
}

// TODO: Won't show opacity/saturation change in reference tool option
void ReferenceDockerDock::setCanvas(KoCanvasBase *canvas) {
    if (canvas) {
        KisView *view = static_cast<KisCanvas2*>(canvas)->imageView();
        if (view) {
            KisDocument *document = view->document();
            if (document && document != m_document) {
                m_document = document;

                // This signal will fire when add/delete/move ref image
                // But oddly won't fire when ref image drop to 0
                connect(document, &KisDocument::sigReferenceImagesChanged, this, &ReferenceDockerDock::loadDefaultOrClear);

                // This signal will fire when ref layer get deleted (when ref image is 0)
                // But oddly won't fire when a ref layer is created
                connect(document, &KisDocument::sigReferenceImagesLayerChanged, this, &ReferenceDockerDock::loadDefaultOrClear);

                // Document startup won't send those signals so we call it once to load the reference image if any
                loadDefaultOrClear();
            }
        }
    }
}
