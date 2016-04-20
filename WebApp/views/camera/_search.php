<?php

use yii\helpers\Html;
use yii\widgets\ActiveForm;

/* @var $this yii\web\View */
/* @var $model app\models\search\CameraSearch */
/* @var $form yii\widgets\ActiveForm */
?>

<div class="camera-search">

    <?php $form = ActiveForm::begin([
        'action' => ['index'],
        'method' => 'get',
    ]); ?>

    <?= $form->field($model, 'id') ?>

    <?= $form->field($model, 'cam_num') ?>

    <?= $form->field($model, 'cam_name') ?>

    <?= $form->field($model, 'cam_folder') ?>

    <?= $form->field($model, 'server_ip') ?>

    <?php // echo $form->field($model, 'location_lat') ?>

    <?php // echo $form->field($model, 'location_lng') ?>

    <?php // echo $form->field($model, 'warning_level') ?>

    <?php // echo $form->field($model, 'cam_status') ?>

    <?php // echo $form->field($model, 'algo_mask') ?>

    <?php // echo $form->field($model, 'algo_status') ?>

    <div class="form-group">
        <?= Html::submitButton('Search', ['class' => 'btn btn-primary']) ?>
        <?= Html::resetButton('Reset', ['class' => 'btn btn-default']) ?>
    </div>

    <?php ActiveForm::end(); ?>

</div>
