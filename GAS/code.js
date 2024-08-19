function doGet(e) {
    try {

        // 温度
        const t = e.parameter.t;
        // 湿度
        const h = e.parameter.h;
        // 日付
        const d = new Date(e.parameter.d);
        // カレンダーにイベント追加
        addEvent(t, h, d);

    } catch (error) {
        // エラーをコンソールに出力
        console.error(error);
        // 失敗したことを return
        return result(false);
    }
    // 成功したことを return
    return result(true);
}