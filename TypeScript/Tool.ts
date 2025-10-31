const PrintObjectMember = (comp) => {
	console.log(`try call on land`);
	console.log(`类名:`, comp.GetClass().GetName());
	try {
		for (let key in comp) {
			// key 是字符串，是属性或函数的名字
			let valueType = typeof (comp as any)[key];
			console.log(`  > 成员: ${key}  (类型: ${valueType})`);
		}
	} catch (e) {
		console.error("遍历成员时出错:", e);
	}
}
