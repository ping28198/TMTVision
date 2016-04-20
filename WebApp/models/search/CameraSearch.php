<?php

namespace app\models\search;

use Yii;
use yii\base\Model;
use yii\data\ActiveDataProvider;
use app\models\camera;

/**
 * CameraSearch represents the model behind the search form about `app\models\camera`.
 */
class CameraSearch extends camera
{
    /**
     * @inheritdoc
     */
    public function rules()
    {
        return [
            [['id', 'cam_num'], 'integer'],
            [['cam_name', 'cam_folder', 'server_ip', 'location_lat', 'location_lng', 'warning_level', 'cam_status', 'algo_mask', 'algo_status'], 'safe'],
        ];
    }

    /**
     * @inheritdoc
     */
    public function scenarios()
    {
        // bypass scenarios() implementation in the parent class
        return Model::scenarios();
    }

    /**
     * Creates data provider instance with search query applied
     *
     * @param array $params
     *
     * @return ActiveDataProvider
     */
    public function search($params)
    {
        $query = camera::find();

        $dataProvider = new ActiveDataProvider([
            'query' => $query,
        ]);

        $this->load($params);

        if (!$this->validate()) {
            // uncomment the following line if you do not want to return any records when validation fails
            // $query->where('0=1');
            return $dataProvider;
        }

        $query->andFilterWhere([
            'id' => $this->id,
            'cam_num' => $this->cam_num,
        ]);

        $query->andFilterWhere(['like', 'cam_name', $this->cam_name])
            ->andFilterWhere(['like', 'cam_folder', $this->cam_folder])
            ->andFilterWhere(['like', 'server_ip', $this->server_ip])
            ->andFilterWhere(['like', 'location_lat', $this->location_lat])
            ->andFilterWhere(['like', 'location_lng', $this->location_lng])
            ->andFilterWhere(['like', 'warning_level', $this->warning_level])
            ->andFilterWhere(['like', 'cam_status', $this->cam_status])
            ->andFilterWhere(['like', 'algo_mask', $this->algo_mask])
            ->andFilterWhere(['like', 'algo_status', $this->algo_status]);

        return $dataProvider;
    }
}
