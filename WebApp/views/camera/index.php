<?php

use yii\helpers\Html;
use yii\grid\GridView;

/* @var $this yii\web\View */
/* @var $searchModel app\models\search\CameraSearch */
/* @var $dataProvider yii\data\ActiveDataProvider */

$this->title = 'Cameras';
$this->params['breadcrumbs'][] = $this->title;
?>
<div class="camera-index">

    <h1><?= Html::encode($this->title) ?></h1>
    <?php // echo $this->render('_search', ['model' => $searchModel]); ?>

    <p>
        <?= Html::a('Create Camera', ['create'], ['class' => 'btn btn-success']) ?>
    </p>

    <?= GridView::widget([
        'dataProvider' => $dataProvider,
        'filterModel' => $searchModel,
        'columns' => [
            ['class' => 'yii\grid\SerialColumn'],

            'id',
            'cam_num',
            'cam_name',
            'cam_folder',
            'server_ip',
            // 'location_lat',
            // 'location_lng',
            // 'warning_level',
            // 'cam_status',
            // 'algo_mask',
            // 'algo_status',

            ['class' => 'yii\grid\ActionColumn'],
        ],
    ]); ?>

</div>
