// レスポンス 生成
function result(status) {
    return ContentService.createTextOutput(
        JSON.stringify({ status: status })
    ).setMimeType(ContentService.MimeType.JSON);
}

// カレンダーに追加
function addEvent(t, h, d) {

    // カレンダーを取得
    const calendar = CalendarApp.getCalendarById(CalendarID);

    // 予定を追加
    calendar.createEvent(EventTitle(t, h), d, d);
}
