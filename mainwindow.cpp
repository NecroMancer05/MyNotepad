#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    setWindowTitle("My Notepad");

    ui->actionSave->setDisabled(true);
    ui->actionSave_As->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{

    if(textChanged){
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setText("The document has been modified.");
        messageBox.setInformativeText("Do you want to save your changes?");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        int ret = messageBox.exec();

        switch (ret) {
          case QMessageBox::Save:
                // Save was clicked

            if (isNewFile == true) {
                //Save as new file
                MainWindow::on_actionSave_As_triggered();
            }else{
                //Overwrite the file
                MainWindow::on_actionSave_triggered();
            }

              //clear and reset
              currentFile.clear();
              ui->textEdit->setText(QString());

              ui->statusBar->showMessage("New File");
              textChanged = false;
              isNewFile = true;

              break;
          case QMessageBox::Discard:
              // Don't Save was clicked
              currentFile.clear();
              ui->textEdit->setText(QString());

              ui->statusBar->showMessage("New File");
              textChanged = false;
              isNewFile = true;
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              messageBox.close();
              break;
          default:
              // should never be reached
              break;
        }
    }

}

void MainWindow::on_actionOpen_triggered()
{

    if(textChanged){
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setText("The document has been modified.");
        messageBox.setInformativeText("Do you want to save your changes?");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        int ret = messageBox.exec();

        switch (ret) {
          case QMessageBox::Save:
                // Save was clicked

            if (isNewFile == true) {
                //Save as new file
                MainWindow::on_actionSave_As_triggered();
            }else{
                //Overwrite the file
                MainWindow::on_actionSave_triggered();
            }

              isNewFile = false;
              textChanged = false;

              MainWindow::on_actionOpen_triggered();

              break;
          case QMessageBox::Discard:
              // Don't Save was clicked
              isNewFile = false;
              textChanged = false;

              MainWindow::on_actionOpen_triggered();
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              messageBox.close();
              break;
          default:
              // should never be reached
              break;
        }
    }else{

        QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
        QFile file(fileName);
        currentFile = fileName;

        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {

            QMessageBox::warning(this, "Warning", "Cannot open file" + file.errorString());
            return;
        }

        setWindowTitle(fileName);
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();

        ui->statusBar->showMessage("Line count : " + QString::number(ui->textEdit->document()->blockCount()));
        isNewFile = false;
        textChanged = false;

        ui->actionSave->setDisabled(true);
        ui->actionSave_As->setEnabled(true);
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as ");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {

        QMessageBox::warning(this, "Warning", "Cannot save file " + file.errorString());
        return;
    }

    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();

    out << text;
    file.close();

    ui->statusBar->showMessage("Saved");
    textChanged = false;
}

void MainWindow::on_actionSave_triggered()
{
    QFile file(currentFile);

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();

    if (!file.open(QFile::WriteOnly | QFile::Text)) {

        QMessageBox::warning(this, "Warning", "Cannot save file " + file.errorString());
        return;
    }

    out << text;
    file.close();

    ui->statusBar->showMessage("Saved");
    textChanged = false;
}

void MainWindow::on_actionExit_triggered()
{
    if(textChanged == true){
        //save

        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setText("The document has been modified.");
        messageBox.setInformativeText("Do you want to save your changes?");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        int ret = messageBox.exec();

        switch (ret) {
          case QMessageBox::Save:
              // Save was clicked

            if (isNewFile == true) {
                //Save as new file
                MainWindow::on_actionSave_As_triggered();
            }else{
                //Overwrite the file
                MainWindow::on_actionSave_triggered();
            }
              break;
          case QMessageBox::Discard:
              // Don't Save was clicked
              QApplication::exit();
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              messageBox.close();
              break;
          default:
              // should never be reached
              break;
        }
    }else{
        QApplication::exit();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->statusBar->showMessage("Copied", 5000);
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
    ui->statusBar->showMessage("Pasted", 3000);
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->statusBar->showMessage("Cutted", 5000);
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
    ui->statusBar->showMessage("Undo", 2500);
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
    ui->statusBar->showMessage("Redo", 2500);
}

void MainWindow::on_textEdit_textChanged()
{
    textChanged = true;

    //TODO : not good !!!
    QString text = ui->textEdit->toPlainText();

    if(text.isEmpty()){

        ui->actionSave->setDisabled(true);
        ui->actionSave_As->setDisabled(true);
    }else{

        if(isNewFile){

            ui->actionSave_As->setEnabled(true);
        }else{
            ui->actionSave_As->setEnabled(true);
            ui->actionSave->setEnabled(true);
        }
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{

    if(textChanged){
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Question);
        messageBox.setText("The document has been modified.");
        messageBox.setInformativeText("Do you want to save your changes?");
        messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        int ret = messageBox.exec();

        switch (ret) {
          case QMessageBox::Save:
                // Save was clicked

            if (isNewFile == true) {
                //Save as new file
                MainWindow::on_actionSave_As_triggered();
            }else{
                //Overwrite the file
                MainWindow::on_actionSave_triggered();
            }

              //clear and reset
              currentFile.clear();
              ui->textEdit->setText(QString());

              ui->statusBar->showMessage("New File");
              textChanged = false;
              isNewFile = true;

              event->accept();
              break;
          case QMessageBox::Discard:
              // Don't Save was clicked
              currentFile.clear();
              ui->textEdit->setText(QString());

              ui->statusBar->showMessage("New File");
              textChanged = false;
              isNewFile = true;
              event->accept();
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              messageBox.close();
              event->ignore();
              break;
          default:
              // should never be reached
              break;
        }
    }else{
        event->accept();
    }
}
