#include "addeditadrenalinenode.h"
#include "ui_addeditadrenalinenode.h"
#include "masternodeconfig.h"
#include "masternodemanager.h"
#include "ui_masternodemanager.h"

#include "walletdb.h"
#include "wallet.h"
#include "ui_interface.h"
#include "util.h"
#include "key.h"
#include "script.h"
#include "init.h"
#include "base58.h"
#include <QMessageBox>
#include <QClipboard>

//Modified at B_CLIENT_VERSION 1.1.0.0
AddEditAdrenalineNode::AddEditAdrenalineNode(std::string editalias, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditAdrenalineNode)
{
    ui->setupUi(this);

    //Labels
    ui->aliasLineEdit->setPlaceholderText("Enter your Masternode alias");
    ui->addressLineEdit->setPlaceholderText("Enter your IP & port");
    ui->privkeyLineEdit->setPlaceholderText("Enter your Masternode private key");
    ui->txhashLineEdit->setPlaceholderText("Enter your 50000 TOBA TXID");
    ui->outputindexLineEdit->setPlaceholderText("Enter your transaction output index");
    ui->rewardaddressLineEdit->setPlaceholderText("Enter a reward recive address");
    ui->rewardpercentageLineEdit->setPlaceholderText("Input the % for the reward");

    sEditAlias = "";
    if (editalias == "")    return;

    //for edit
    BOOST_FOREACH(CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
        if(mne.getAlias() == editalias) {
            ui->aliasLineEdit->setText(QString::fromStdString(mne.getAlias()));
            ui->addressLineEdit->setText(QString::fromStdString(mne.getIp()));
            ui->privkeyLineEdit->setText(QString::fromStdString(mne.getPrivKey()));
            ui->txhashLineEdit->setText(QString::fromStdString(mne.getTxHash()));
            ui->outputindexLineEdit->setText(QString::fromStdString(mne.getOutputIndex()));
            ui->rewardaddressLineEdit->setText(QString::fromStdString(mne.getRewardAddress()));
            ui->rewardpercentageLineEdit->setText(QString::fromStdString(mne.getRewardPercentage()));
            
            sEditAlias = editalias;
            return;
        }
    }
    return;

}

AddEditAdrenalineNode::~AddEditAdrenalineNode()
{
    delete ui;
}

void AddEditAdrenalineNode::on_okButton_clicked()
{
    if(ui->aliasLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter an alias.");
        msg.exec();
        return;
    }
    else if(ui->addressLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter an ip address and port. (123.45.67.89:9999)");
        msg.exec();
        return;
    }
    else if(ui->privkeyLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter a masternode private key. This can be found using the \"masternode genkey\" command in the console.");
        msg.exec();
        return;
    }
    else if(ui->txhashLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter the transaction hash for the transaction that has 500 coins");
        msg.exec();
        return;
    }
    else if(ui->outputindexLineEdit->text() == "")
    {
        QMessageBox msg;
        msg.setText("Please enter a transaction output index. This can be found using the \"masternode outputs\" command in the console.");
        msg.exec();
        return;
    }
    else
    {
        std::string sAlias = ui->aliasLineEdit->text().toStdString();
        std::string sAddress = ui->addressLineEdit->text().toStdString();
        std::string sMasternodePrivKey = ui->privkeyLineEdit->text().toStdString();
        std::string sTxHash = ui->txhashLineEdit->text().toStdString();
        std::string sOutputIndex = ui->outputindexLineEdit->text().toStdString();
        std::string sRewardAddress = ui->rewardaddressLineEdit->text().toStdString();
        std::string sRewardPercentage = ui->rewardpercentageLineEdit->text().toStdString();

        //Modified at B_CLIENT_VERSION 1.1.0.0
        if (sEditAlias != "")    masternodeConfig.remove(sEditAlias);	//when edit
        
        masternodeConfig.add(sAlias, sAddress, sMasternodePrivKey, sTxHash, sOutputIndex, sRewardAddress, sRewardPercentage);
        masternodeConfig.writeAll();
        
        accept();
    }
}

void AddEditAdrenalineNode::on_cancelButton_clicked()
{
    reject();
}

void AddEditAdrenalineNode::on_AddEditAddressPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->addressLineEdit->setText(QApplication::clipboard()->text());
}

void AddEditAdrenalineNode::on_AddEditPrivkeyPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->privkeyLineEdit->setText(QApplication::clipboard()->text());
}

void AddEditAdrenalineNode::on_AddEditTxhashPasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->txhashLineEdit->setText(QApplication::clipboard()->text());
}
