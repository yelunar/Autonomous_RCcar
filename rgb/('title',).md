serializer 할때

무비리스트시리얼라이저, 무비타이틀시리얼라이저 액터리스트시리얼라이저 이런거는 그냥 정의함

class MovieTitleSerializer(serializers.ModelSerializer):
    class Meta:
        model = Movie
        fields = '__all__' # ('title',)

M:N관계를 맺고있는거는 따로 처리 해줘야함

추후에  Actor를 생성하는 view는 만들어져 있지 않지만, 추후에 추가될 수 있으므로 미리 읽기 전용 필드로 적용

class ActorSerializer(serializers.ModelSerializer):
    movies = MovieTitleSrializer(many=True, read_only=True)
    class Meta:
        model = Actor
        fields = ('id', 'name', 'movies')

 단, 역참조중인 배우의 이름도 표시하기 위하여 별도의 serializer를 정의하여 사용중

 또한, 역참조중인 리뷰 목록은 리뷰목록 시리얼라이저로 제공

class MovieSerializer(serializers.ModelSerializer):
    class ActorSerializerForMovie(serializers.ModelSerializer):
        class Meta:
            model = Actor
            fields = ('name',)
    actors = ActorSerializerForMovie(many=True)
    review_set = ReviewListSerializer(many=True)
    class Meta:
        model = Movie
        fields = '__all__'

---

views.py 할때

from .serializers import ActorListSerializer, ActorSerializer, MovieSerializer, MovieListSerializer, ReviewSerializer, ReviewListSerializer

이런거 다 임포트 해줘야함!

```python
@api_view(['GET'])
def actor_list(request):
    actors = Actor.objects.all()
    serializer = ActorListSerializer(actors, many=True)
    return Response(serializer.data)

@api_view(['GET'])
def actor_detail(request, actor_pk):
    actor = get_object_or_404(Actor, pk=actor_pk) # pk받으려면이렇
    serializer = ActorSerializer(actor)
    return Response(serializer.data)

@api_view(['GET', 'POST'])
def book_list(request):
    if request.method == 'GET':
        books = Book.objects.all() # 1. db 에서 데이터 가져온다 
        serializer = BookListSerializer(books, many=True) # 2. 가져온 데이터를 만든 시리얼라이저
        return Response(serializer.data) # 3. 작성된 시리얼라이저의 데이터를 응답으로 리턴

    elif request.method == 'POST':
        serializer = BookSerializer(data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)


@api_view(['GET', 'DELETE', 'PUT'])
def music_detail(request, music_pk):
    music = get_object_or_404(Music, pk=music_pk)
    if request.method == 'GET':
        serializer = MusicSerializer(music)
        return Response(serializer.data)

    elif request.method == 'DELETE':
        music.delete()
        data = {
            'delete': f'데이터 {music_pk}번 음악이 삭제되었습니다.',
        }
        return Response(data, status=status.HTTP_204_NO_CONTENT)

    elif request.method == 'PUT':
        serializer = MusicSerializer(music, data=request.data)
        if serializer.is_valid(raise_exception=True):
            serializer.save()
            return Response(serializer.data)

### POST만

@api_view(['POST'])
def comment_create(request, music_pk):
    music = get_object_or_404(Music, pk=music_pk)

    serializer = CommentSerializer(data=request.data)
    if serializer.is_valid(raise_exception=True):
        serializer.save(music=music)
        return Response(serializer.data, status=status.HTTP_201_CREATED)
```


