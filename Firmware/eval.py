import pandas as pd
import re

# Đường dẫn đến file RTF
file_path = 'E:/GitHub/Bachelor_Project/Reference/testing_scenario/Dgia_do_tin_cay/5phut_01.rtf'

# Đọc nội dung file RTF
with open(file_path, 'r', encoding='utf-8') as file:
    content = file.read()

# Sử dụng regex để trích xuất dữ liệu
pattern = re.compile(r'(\d+), (\d+), (\d+), ([\d-]+), (\w)')
matches = pattern.findall(content)

# Chuyển dữ liệu thành DataFrame
data = pd.DataFrame(matches, columns=['Timestamp', 'Value1', 'Value2', 'Value3', 'Label'])
data[['Timestamp', 'Value1', 'Value2', 'Value3']] = data[['Timestamp', 'Value1', 'Value2', 'Value3']].astype(int)

# Ghép cặp các dòng dữ liệu lại với nhau
data['Sample'] = (data.index // 2) + 1

# Tạo DataFrame mới cho từng mẫu
samples = data.groupby('Sample').agg({
    'Value1': ['mean', 'std'],
    'Value2': ['mean', 'std'],
    'Value3': ['mean', 'std'],
    'Label': lambda x: x.mode()[0]  # Lấy nhãn phổ biến nhất
}).reset_index()

# Đổi tên các cột
samples.columns = ['Sample', 'Value1_mean', 'Value1_std', 'Value2_mean', 'Value2_std', 'Value3_mean', 'Value3_std', 'Label']

# Kiểm tra các mẫu để tìm sai số rời rạc
# Giả sử các giá trị ngoại lệ nằm ngoài 3 lần độ lệch chuẩn so với giá trị trung bình
outliers = samples[
    (samples['Value1_std'] > 3 * samples['Value1_std'].mean()) |
    (samples['Value2_std'] > 3 * samples['Value2_std'].mean()) |
    (samples['Value3_std'] > 3 * samples['Value3_std'].mean())
]

# Hiển thị các mẫu có sai số rời rạc
print("Mẫu có sai số rời rạc:")
print(outliers)

# Kiểm tra kết quả
unique_labels = samples['Label'].unique()
print("Các nhãn duy nhất trong dữ liệu:", unique_labels)