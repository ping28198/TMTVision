<?php
use yii\helpers\Html;
use yii\widgets\ActiveForm;

/* @var $this yii\web\View */
?>
<h1>monitor/index</h1>

<p>
    You may change the content of this page by modifying
    the file <code><?= __FILE__; ?></code>.
</p>
<?php $form = ActiveForm::begin();  ?>
<?= $form->field($model,'cam_id')->dropDownList(['1'=>1,'2'=>2]); ?>
<?= $form->field($model,'date')->input('date') ?>
<div class="form-group">
<?= Html::submitButton('查看',['class'=>'btn btn-primary']) ?>
</div>
<?php ActiveForm::end(); ?>
