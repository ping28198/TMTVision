<?php

namespace app\controllers;

use Yii;
use app\models\camera;
use app\models\image;
use app\models\search\CameraSearch;
use yii\web\Controller;
use yii\web\NotFoundHttpException;
use yii\filters\VerbFilter;

class MonitorController extends \yii\web\Controller
{
    public function actionIndex()
    {
        $a = new image();
        return $this->render('index');
    }

    public function actionDetail($cam_id)
    {
        $camera = Camera::findOne($cam_id);
        $cam_images = $camera->getImages()->all();
        //$model = $this->findModel($cam_id);

        return $this->render('detail',[
            'cam_images'=>$cam_images
        ]);
    }

    public function actionHistory()
    {
        return $this->render('history');
    }

    public function actionOnline()
    {
        return $this->render('online');
    }

    /**
     * Finds the image model based on its primary key value.
     * If the model is not found, a 404 HTTP exception will be thrown.
     * @param integer $id
     * @return image the loaded model
     * @throws NotFoundHttpException if the model cannot be found
     */
    protected function findModel($id)
    {
        if (($model = image::findOne($id)) !== null)
        {
            return $model;
        }
        else
        {
            throw new NotFoundHttpException('The requested page does not exist.');
        }
    }

    /***
     * 查找某一个相机下的所有图像
     * @param $cam_id
     */
    private function findAllImages($cam_id)
    {
        $table_name = Image::tableName();
//        $all_images = Image::findBySql("
//SELECT * FROM `camera`,{$table_name}
//WHERE `{$table_name}`.`cam_id`={$cam_id}
//AND `camera`.`id` = `{$table_name}`.`cam_id`
//")->all();
        //TODO:这里需要改，增加关联 参考地址：http://www.yiiframework.com/doc-2.0/guide-db-active-record.html#relational-data
        $all_images = Image::find()->joinWith(['camera'])->where(['cam_id' => $cam_id])->all();
        return $all_images;
    }

}
