<?php

use yii\helpers\Html;
use yii\widgets\ActiveForm;

/* @var $this yii\web\View */
/* @var $model app\models\camera */
/* @var $form yii\widgets\ActiveForm */
?>

<div class="camera-form">

    <?php $form = ActiveForm::begin(); ?>

    <?= $form->field($model, 'cam_num')->textInput() ?>

    <?= $form->field($model, 'cam_name')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'cam_folder')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'server_ip')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'location_lat')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'location_lng')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'warning_level')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'cam_status')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'algo_mask')->textInput(['maxlength' => true]) ?>

    <?= $form->field($model, 'algo_status')->textInput(['maxlength' => true]) ?>

    <div class="form-group">
        <?= Html::submitButton($model->isNewRecord ? 'Create' : 'Update', ['class' => $model->isNewRecord ? 'btn btn-success' : 'btn btn-primary']) ?>
    </div>

    <?php ActiveForm::end(); ?>

</div>
