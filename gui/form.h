//
// Created by drin on 28/12/2024.
//

#ifndef RJMMO_FORM_H
#define RJMMO_FORM_H

#include "model.h"
#include "utils.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

template <typename T>
class Form : public QDialog {
 public:
  explicit Form(QWidget* parent, T* data, bool insert = false)
      : QDialog(parent), data_(data), insert_(insert) {
    set_form();
  }

 private:
  T* data_;
  bool insert_;
  const std::string IS_NULL_SUFFIX = "_is_null";

  void set_form() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QDialogButtonBox* btn_box = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);

    for (const Column<T>& col : T::table().columns) {
      QHBoxLayout* label_row = new QHBoxLayout();
      layout->addLayout(label_row);

      QLabel* label = new QLabel(col.label().data(), this);
      label_row->addWidget(label);

      auto elem = gen_form_elem(col);
      if (elem.second == nullptr) {
        alert_err("Something went wrong while generating the form.");
        return;
      }
      if (col.nullable()) {
        QCheckBox* check_box = new QCheckBox(this);
        check_box->setObjectName(col.name() + IS_NULL_SUFFIX);
        check_box->setChecked(elem.first);
        label_row->addWidget(check_box);
        label_row->addStretch();
        elem.second->setEnabled(elem.first);
        elem.second->setVisible(elem.first);

        connect(check_box, &QCheckBox::toggled, this,
                [this, elem = elem.second](bool checked) {
                  elem->setEnabled(checked);
                  elem->setVisible(checked);
                });
      }
      layout->addWidget(elem.second);
    }
    connect(btn_box, &QDialogButtonBox::accepted, this, &Form::accept);
    connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(btn_box);
    adjustSize();
  }

  std::pair<bool, QWidget*> gen_form_elem(const Column<T>& col) {
    switch (col.type()) {
      case ColumnType::INTEGER: {
        QSpinBox* sb = new QSpinBox(this);
        sb->setMinimum(col.imin());
        sb->setMaximum(col.imax());
        sb->setObjectName(col.name());
        bool has_val = false;
        if (col.nullable() &&
            std::get<IntOpt>(col.getter(*data_)).has_value()) {
          has_val = true;
          sb->setValue(std::get<IntOpt>(col.getter(*data_)).value());
        } else if (!col.nullable() && !insert_) {
          sb->setValue(std::get<int>(col.getter(*data_)));
          has_val = true;
        }
        return std::make_pair(has_val, sb);
      }
      case ColumnType::REAL: {
        QDoubleSpinBox* dsb = new QDoubleSpinBox(this);
        dsb->setMinimum(col.dmin());
        dsb->setMaximum(col.dmax());
        dsb->setDecimals(col.decimal());
        dsb->setObjectName(col.name());
        bool has_val = false;
        if (col.nullable() &&
            std::get<DoubleOpt>(col.getter(*data_)).has_value()) {
          has_val = true;
          dsb->setValue(std::get<DoubleOpt>(col.getter(*data_)).value());
        } else if (!col.nullable() && !insert_) {
          dsb->setValue(std::get<double>(col.getter(*data_)));
          has_val = true;
        }
        return std::make_pair(has_val, dsb);
      }
      case ColumnType::VARCHAR: {
        QLineEdit* line_edit = new QLineEdit(this);
        line_edit->setMaxLength(col.vlen());
        line_edit->setObjectName(col.name());
        bool has_val = false;
        if (col.nullable() &&
            std::get<StrViewOpt>(col.getter(*data_)).has_value()) {
          has_val = true;
          line_edit->setText(
              std::get<StrViewOpt>(col.getter(*data_)).value().data());
        } else if (!col.nullable() && !insert_) {
          line_edit->setText(
              std::get<std::string_view>(col.getter(*data_)).data());
          has_val = true;
        }
        return std::make_pair(has_val, line_edit);
      }
      case ColumnType::DATE: {
        QDateEdit* date_edit = new QDateEdit(this);
        date_edit->setDisplayFormat("yyyy-MM-dd");
        date_edit->setCalendarPopup(true);
        date_edit->setObjectName(col.name());
        bool has_val = false;
        if (col.nullable() &&
            std::get<StrViewOpt>(col.getter(*data_)).has_value()) {
          date_edit->setDate(QDate::fromString(
              std::get<StrViewOpt>(col.getter(*data_)).value().data(),
              Qt::ISODate));
          has_val = true;
        } else if (!col.nullable() && !insert_) {
          date_edit->setDate(QDate::fromString(
              std::get<std::string_view>(col.getter(*data_)).data(),
              Qt::ISODate));
          has_val = true;
        }
        return std::make_pair(has_val, date_edit);
      }
      case ColumnType::FK: {
        QComboBox* cb = new QComboBox(this);
        cb->setObjectName(col.name());
        auto list = ModelRegistry::get_all_as_str(col.fk_ref());
        if (!list.has_value()) {
          alert_err("Something went wrong while fetching data.");
          return std::make_pair(false, nullptr);
        }
        for (const auto& pair : list.value()) {
          cb->addItem(pair.second.data(), pair.first);
          if ((std::get<ForeignKeyRef>(col.getter(*data_)).get().second ==
               pair.second) &&
              !insert_) {
            cb->setCurrentIndex(cb->count() - 1);
          }
        }
        return std::make_pair(true, cb);
      }
      default:
        return std::make_pair(false, nullptr);
    }
  }

  void accept() override {
    for (const Column<T>& col : T::table().columns) {
      auto elem = findChild<QWidget*>(col.name());
      FieldSetter val;
      if (QSpinBox* sp = qobject_cast<QSpinBox*>(elem)) {
        if (col.nullable() &&
            !findChild<QCheckBox*>(col.name() + IS_NULL_SUFFIX)->isChecked()) {
          val = std::nullopt;
        }
        val = sp->value();
      } else if (QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(elem)) {
        if (col.nullable() &&
            !findChild<QCheckBox*>(col.name() + IS_NULL_SUFFIX)->isChecked()) {
          val = std::nullopt;
        }
        val = dsp->value();
      } else if (QLineEdit* le = qobject_cast<QLineEdit*>(elem)) {
        if (col.nullable() &&
            !findChild<QCheckBox*>(col.name() + IS_NULL_SUFFIX)->isChecked()) {
          val = std::nullopt;
        }
        val = le->text().toStdString();
      } else if (QDateEdit* de = qobject_cast<QDateEdit*>(elem)) {
        if (col.nullable() &&
            !findChild<QCheckBox*>(col.name() + IS_NULL_SUFFIX)->isChecked()) {
          val = std::nullopt;
        } else {
          val = de->date().toString(Qt::ISODate).toStdString();
        }
      } else if (QComboBox* cb = qobject_cast<QComboBox*>(elem)) {
        val = ForeignKey(cb->currentData().toInt(),
                         cb->currentText().toStdString());
      }
      col.setter(*data_, val);
    }
    bool result = insert_ ? data_->insert() : data_->update();
    if (!result) {
      alert_err("An error occurred while trying to " +
                QString(insert_ ? "create" : "update") + " the item.");
      QDialog::reject();
    } else {
      QDialog::accept();
    }
  }
};

#endif  //RJMMO_FORM_H
