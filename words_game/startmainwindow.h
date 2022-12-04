#ifndef STARTMAINWINDOW_H
#define STARTMAINWINDOW_H

#include <QMainWindow>
#include"word.h"

namespace Ui {
class StartMainWindow;
}

class StartMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartMainWindow(QWidget *parent = nullptr);
    void get_next_words();
    ~StartMainWindow();
signals:
    void backToMainWindow();
    void startGame();




private slots:

    void on_pushButton_clicked();
    void on_word_19_clicked(bool checked);
    void readWordsFile();

    void on_inputWords_textChanged();

    void on_pushButton_inputWord_clicked();


    void on_word_13_clicked();

    void on_word_1_clicked();

    void on_word_2_clicked();

    void on_word_3_clicked();

    void on_word_4_clicked();

    void on_word_5_clicked();

    void on_word_6_clicked();

    void on_word_7_clicked();

    void on_word_8_clicked();

    void on_word_9_clicked();

    void on_word_10_clicked();

    void on_word_11_clicked();

    void on_word_12_clicked();

    void on_word_14_clicked();

    void on_word_15_clicked();

    void on_word_16_clicked();

    void on_word_17_clicked();

    void on_word_18_clicked();

    void on_word_19_clicked();

    void on_word_20_clicked();

    void on_choiceWordsText_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_NowWord_windowIconTextChanged(const QString &iconText);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::StartMainWindow *ui;
    word word_control;
    bool is_first_click;
    int show_word_num;//想要显示的单词数量
    int word_num;//有效接龙单词数
    long int goals;
    //bool is_overlap_goal;
    //bool is_depend_less_words;


};

#endif // STARTMAINWINDOW_H
